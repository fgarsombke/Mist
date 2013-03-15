#include "LawnSimStd.h"

#include "Yard.h"
#include "WeatherData.h"
#include "ETCalc.h"

#define EdgeProject 1

namespace Mist { namespace LawnSim {

   
Yard::Yard(const YardInfo& yardInfo)
   : locale_(yardInfo.locale()), cells_(InitCells(yardInfo)), 
     cells_by_height_(InitHeightMap(yardInfo)),
     sprinklers_(std::move(yardInfo.sprinklers())),
	 surface_water_(cells_.size1(), cells_.size2(), 1)
{
   // Zero everything out and
   // Generate a height sorted view of the Yard
   

   // Generate rain mask
   // TODO: Should this be nonideal?
   rain_mask_ = bnu::scalar_matrix<double>(cells_.size1() - 2, cells_.size2() - 2, 1.0);

   // TODO: Implement
   sprinkler_masks_ = SprinklerMaskList_t(sprinklers_.size());
   for (SprinklerMask_t &mask : sprinkler_masks_) {
      mask = SprinklerMask_t(cells_.size1() - 2, cells_.size2() - 2);
   }
}



const bnu::matrix<YardCell> Yard::InitCells(const YardInfo& yardInfo)
{
   if (yardInfo.yard_length() == 0 || yardInfo.yard_width() == 0) {
      throw std::invalid_argument("Yard Length and Width must be nonzero.");
   }

   // First compute the constant values, mostly simulation parameters
   bnu::matrix<YardCell> cells = bnu::matrix<YardCell>(yardInfo.yard_length() + 2, yardInfo.yard_width() + 2);
   auto cellInfos = yardInfo.yard_cells();

   size_t lstRow = cells.size1() - 1;
   size_t lstCol = cells.size2() - 1;

   size_t lstInrRow = cellInfos.size1() - 1;
   size_t lstInrCol = cellInfos.size2() - 1;

   // Assign Void Corner Height
   cells(0,0)           = YardCell::CreateVoid(cellInfos(0,0).rel_height());
   cells(0,lstCol)      = YardCell::CreateVoid(cellInfos(0,lstInrCol).rel_height());
   cells(lstRow,0)      = YardCell::CreateVoid(cellInfos(lstInrRow,0).rel_height());
   cells(lstRow,lstCol) = YardCell::CreateVoid(cellInfos(lstInrRow,lstInrCol).rel_height());

   // Assign Void Top and Bottom Row Heights
   for (size_t j = 0; j <= lstInrCol; ++j) {
      cells(0, j + 1)      = YardCell::CreateVoid(cellInfos(0,j).rel_height());
      cells(lstRow, j + 1) = YardCell::CreateVoid(cellInfos(lstInrRow,j).rel_height());
   }

   // Assign Void First and Last Column Heights
   for (size_t i = 0; i <= lstInrCol; ++i) {
      cells(i + 1, 0)      = YardCell::CreateVoid(cellInfos(i, 0).rel_height());
      cells(i + 1, lstCol) = YardCell::CreateVoid(cellInfos(i, lstInrCol).rel_height());
   }


   // At this point each void cell has a height equal to its next "inner" neighbor
   // We could stop assigning here and that would be enough for the "EdgeWall" case

   #if EdgeProject == 1

   // We continue by extrapolating slope into the void

   // Extrapolate first and last columns
   // Only need to extrapolate if there is data to use
   if (cellInfos.size2() > 1) {
      for (size_t i = 0; i <= lstInrRow; ++i) {
         {
            double heightDiffF = cellInfos(i, 0).rel_height() - cellInfos(i, 1).rel_height();
            cells(i + 1, 0).ChangeHeight(heightDiffF);
         }
         {
            double heightDiffL = cellInfos(i, lstInrCol).rel_height() - cellInfos(i, lstInrCol-1).rel_height();
            cells(i + 1, lstCol).ChangeHeight(heightDiffL);
         }
      }
   }

   // Extrapolate first and last rows
   // Similar to the first and last columns
   if (cellInfos.size1() > 1) {
      for (size_t j = 0; j <= lstInrCol; ++j) {
         {
            double heightDiffT = cellInfos(0, j).rel_height() - cellInfos(1, j).rel_height();
            cells(0, j+1).ChangeHeight(heightDiffT);
         }
         {
            double heightDiffB = cellInfos(lstInrRow, j).rel_height() - cellInfos(lstInrRow-1, j).rel_height();
            cells(lstRow, j+1).ChangeHeight(heightDiffB);
         }
      }
   }

   // To each corner we add the average difference between the inner point and the edges
   cells(0,0).ChangeHeight((cells(1,0).cell_info().rel_height() + cells(0,1).cell_info().rel_height() - 2*cells(0,0).cell_info().rel_height())/3);
   cells(0,lstCol).ChangeHeight((cells(1,lstCol).cell_info().rel_height() + cells(0,lstCol-1).cell_info().rel_height() - 2*cells(0,lstCol).cell_info().rel_height())/3);
   cells(lstRow,0).ChangeHeight((cells(lstRow,1).cell_info().rel_height() + cells(lstRow-1,0).cell_info().rel_height() - 2*cells(lstRow,0).cell_info().rel_height())/3);
   cells(lstRow,lstCol).ChangeHeight((cells(lstRow-1,lstCol).cell_info().rel_height() + cells(lstRow,lstCol-1).cell_info().rel_height() - 2*cells(lstRow,lstCol).cell_info().rel_height())/3);

   // Fill the center of the map with heights but bogus cells
   for (size_t row = 1; row < lstRow; ++row) {
      for (size_t col = 1; col < lstCol; ++col) {
         cells(row, col) = YardCell::CreateIsolated(cellInfos(row-1, col-1));
      }
   }


   // Fill in the actual drift map
   //
   NeighborHeightDiffs_t rh;
   rh = 0;

   for (size_t row = 1; row < lstRow; ++row) {
      for (size_t col = 1; col < lstCol; ++col) {
         YardCell myCell = cells(row, col);
         const YardCellInfo myInfo = myCell.cell_info();

         rh.Right() = cells(row, col+1).cell_info().rel_height() - myInfo.rel_height();
         rh.TopRight() = cells(row - 1, col + 1).cell_info().rel_height() - myInfo.rel_height();
         rh.Top() = cells(row - 1, col).cell_info().rel_height() - myInfo.rel_height();
         rh.TopLeft() = cells(row - 1, col - 1).cell_info().rel_height() - myInfo.rel_height();
         rh.Left() = cells(row, col - 1).cell_info().rel_height() - myInfo.rel_height();
         rh.BottomLeft() = cells(row + 1, col - 1).cell_info().rel_height() - myInfo.rel_height();
         rh.Bottom() = cells(row + 1, col).cell_info().rel_height() - myInfo.rel_height();
         rh.BottomRight() = cells(row + 1, col + 1).cell_info().rel_height() - myInfo.rel_height();

         cells(row, col).UnIsolate(rh);
         rh = 0;
      }
   }

   #endif // EdgeWall

   return cells;
}



const bnu::unbounded_array<LawnCoordinate> Yard::InitHeightMap(YardInfo const &yardInfo) {
   bnu::unbounded_array<LawnCoordinate> retVal(yardInfo.yard_length() * yardInfo.yard_width());

   // Need to add one since we're precomputing for the whole yard, not just the grass.

   // Init size array
   for (unsigned int i = 0; i < retVal.size(); ++i) {
      retVal[i] = LawnCoordinate(i/((unsigned int)yardInfo.yard_length()) + 1,i%((unsigned int)yardInfo.yard_width()) + 1);
   }

   // Sort the drift map by height
   bnu::matrix<YardCellInfo> yardCells = yardInfo.yard_cells();
   auto heightLambda = [&yardCells](LawnCoordinate lPos, LawnCoordinate rPos) {
      return yardCells(lPos.Row - 1, lPos.Col - 1).rel_height() >
                     yardCells(rPos.Row - 1, rPos.Col - 1).rel_height();
   };

   std::sort(retVal.begin(), retVal.end(), heightLambda);  

   return retVal;
}

void Yard::ResetState() {
   // Initialize each cell to its default state
   for (YardCell &cell : cells_.data()) {
      cell.ResetState();
   }
   
   // Remove the surface water
   surface_water_.clear();
}


void Yard::ElapseTime(pt::time_period tickPeriod, const WeatherData &data, const std::vector<pt::time_duration> sprinklerDurations) {
   // TODO: Change the api so that sprinklerDurations cannot be accidentally resized
   using namespace bnu;

   FAO_ET::ETCalcParameters params;

   cout << "ElapseTime: " << tickPeriod << endl;

   //static int ticknum = 0;
   //DebugPrintMatrix(surface_water_, "SurfaceWaterVals/SurfaceWater" + std::to_string(ticknum++) + ".csv");
   

   double dt = tickPeriod.length().ticks()/((double)tickPeriod.length().ticks_per_second());

   // Add sprinkler water to the surface
   for (size_t i = 0; i < sprinkler_masks_.size(); ++i) {
      pt::time_duration dt_s = sprinklerDurations[i];
      noalias(matrix_range<matrix<double> >(surface_water_, 
                                             range(1, surface_water_.size1() - 1), 
                                             range(1, surface_water_.size2() - 1)))
         += ((dt_s.ticks()/((double)dt_s.ticks_per_second())) * sprinkler_masks_[i]);
   }

   // Add rain water to the surface
   if (data.rainfall().is_initialized()) {
      noalias(matrix_range<matrix<double> >(surface_water_, 
                                             range(1, surface_water_.size1() - 1), 
                                             range(1, surface_water_.size2() - 1)))
         += (data.rainfall().get() * rain_mask_);
   }
   
   //Redistribute the water over the surface
   for (LawnCoordinate pos : cells_by_height_) {
      matrix_range<matrix<double> >(surface_water_, range(pos.Row - 1, pos.Row + 2), range(pos.Col - 1, pos.Col + 2))
         += surface_water_(pos.Row, pos.Col)*cells_(pos.Row, pos.Col).drift_entry().data();
   }

   // Shine sunlight
   
   // Apply heat
   
   // Apply humidity
   
   // Blow wind

   // Calculate ET_0


   // Grow
   DoGrow(0);
}

inline void Yard::DoGrow(const double ET_0) 
{
   // Grow the grass in the yard
   for (YardCell &cell : cells_.data()) {
      // Calculate ETo metric
   }

}

#ifdef _DEBUG
void Yard::DebugPrint() const
{
   for(unsigned int i = 0; i < cells_.size1(); ++i) {
      for(unsigned int j = 0; j < cells_.size2(); ++j) {
         cout << "Yard Cell (" << i << ", " << j << "): " << endl;
         cout << "\t" << "Type: " << (int)cells_(i,j).cell_type() << endl;
         
         if (cells_(i,j).cell_type() == YardCellType_t::Grass) {
            cout << cells_(i,j).drift_entry() << endl;
         }
      }
   }
}

template<class T>
static void Yard::DebugPrintMatrix(const bnu::matrix<T> &toPrint, std::string fileName)
{
   using namespace std;

   ofstream dbgFile;

   const std::string &name = fileName.length() == 0? "Matrix.csv": fileName;

   dbgFile.open(name);

   for(unsigned int i = 0; i < toPrint.size1(); ++i) {
      for(unsigned int j = 0; j < toPrint.size2() - 1; ++j) {
         dbgFile << toPrint(i,j) << ", ";
      }

      dbgFile << toPrint(i,toPrint.size2() - 1) << endl;
   }

   dbgFile.close();
}

void Yard::DebugPrintHeights(std::string fileName) const
{
   using namespace std;

   ofstream dbgFile;

   const std::string &name = fileName.length() == 0? "Heights.csv": fileName;

   dbgFile.open(name);

   for(unsigned int i = 0; i < cells_.size1(); ++i) {
      for(unsigned int j = 0; j < cells_.size2() - 1; ++j) {
         dbgFile << cells_(i,j).cell_info().rel_height() << ", ";
      }

      dbgFile << cells_(i,cells_.size2() - 1).cell_info().rel_height() << endl;
   }

   dbgFile.close();
}

#else
void Yard::DebugPrint() const
{
}

template<class T>
void Yard::DebugPrintMatrix(const bnu::matrix<T> &toPrint, std::string fileName)
{
}

void Yard::DebugPrintHeights(std::string fileName) const
{
}

#endif




}
}

