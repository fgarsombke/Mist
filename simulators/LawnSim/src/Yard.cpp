#include "LawnSimStd.h"

#include "Yard.h"
#include "WeatherData.h"

namespace Mist {
namespace LawnSim {

   
Yard::Yard(const YardInfo& yardInfo)
   : cells_(InitCells(yardInfo)), cells_by_height_(InitHeightMap(cells_)), 
     locale_(yardInfo.locale()), 
     surface_water_(yardInfo.yard_length(), yardInfo.yard_width(), 0),
     sprinklers_(std::move(yardInfo.sprinklers()))
{
   // Zero everything out and
   // Generate a height sorted view of the Yard


   // Generate rain mask
   // TODO: Should this be nonideal?
   rain_mask_ = bm::scalar_matrix<double>(cells_.size1(), cells_.size2(), 1.0);

   // TODO: Implement
   sprinkler_masks_ = SprinklerMaskList_t(sprinklers_.size());
   for (SprinklerMask_t &mask : sprinkler_masks_) {
      mask = SprinklerMask_t(cells_.size1(), cells_.size2());
   }
}


   
const bm::matrix<YardCell> Yard::InitCells(const YardInfo& yardInfo)
{
   if (yardInfo.yard_length() == 0 || yardInfo.yard_width() == 0) {
      throw std::invalid_argument("Yard Length and Width must be nonzero.");
   }

   // First compute the constant values, mostly simulation parameters

   bm::matrix<YardCell> cells = bm::matrix<YardCell>(yardInfo.yard_length(), yardInfo.yard_width(), YardCell());
   auto cellInfos = yardInfo.yard_cells();

   // Fill in the drift map
   //
   NeighborHeightDiffs_t rh;
   YardCellInfo myInfo;

   // Check corner cases
   rh = 0;
   myInfo = cellInfos(0, 0);

   size_t lstRow = cellInfos.size1() - 1;
   size_t lstCol = cellInfos.size2() - 1;

   if (cells.size1() == 1) {
      if (cells.size1() != 1) {
         // One rown, multiple columns
         myInfo = cellInfos(0, lstCol);
         rh.SetLeft(cellInfos(0, lstCol - 1).rel_height() - myInfo.rel_height());
         cells(0, lstCol) = YardCell(myInfo, rh);

         myInfo = cellInfos(0,0);
         rh.SetLeft(0);
         rh.SetRight(cellInfos(0, 1).rel_height() - myInfo.rel_height());
         cells(0,0) = YardCell(myInfo, rh);
      } else {
         cells(0,0) = YardCell(myInfo, rh);
         return cells;
      }
   } else if (cells.size2() == 1) {
      // Multiple rows, one column
      rh.SetBottom(cellInfos(1, 0).rel_height() - myInfo.rel_height());
      cells(0, 0) = YardCell(myInfo, rh);

      myInfo = cellInfos(lstRow,0);
      rh.SetBottom(0);
      rh.SetTop(cellInfos(lstRow - 1, 0).rel_height() - myInfo.rel_height());

      cells(lstRow,0) = YardCell(myInfo, rh);
   } else { 
      // Matrix is 2x2 or larger
      // Do corners
      rh.SetRight(cellInfos(0, 1).rel_height() - myInfo.rel_height());
      rh.SetBottomRight(cellInfos(1, 1).rel_height() - myInfo.rel_height());
      rh.SetBottom(cellInfos(1, 0).rel_height() - myInfo.rel_height());
      cells(0,0) = YardCell(myInfo, rh);
      rh = 0;
      
      myInfo = cellInfos(0, lstCol);
      rh.SetLeft(cellInfos(0, lstCol - 1).rel_height() - myInfo.rel_height());
      rh.SetBottomLeft(cellInfos(1, lstCol - 1).rel_height() - myInfo.rel_height());
      rh.SetBottom(cellInfos(1, lstCol).rel_height() - myInfo.rel_height());
      cells(0, lstCol) = YardCell(myInfo, rh);
      rh = 0;
      
      myInfo = cellInfos(lstRow, 0);
      rh.SetTop(cellInfos(lstRow - 1, 0).rel_height() - myInfo.rel_height());
      rh.SetTopRight(cellInfos(lstRow - 1, 1).rel_height() - myInfo.rel_height());
      rh.SetRight(cellInfos(lstRow, 1).rel_height() - myInfo.rel_height());
      cells(lstRow, 0) = YardCell(myInfo, rh);
      rh = 0;

      myInfo = cellInfos(lstRow, lstCol);
      rh.SetTop(cellInfos(lstRow - 1, lstCol).rel_height() - myInfo.rel_height());
      rh.SetTopLeft(cellInfos(lstRow - 1, lstCol - 1).rel_height() - myInfo.rel_height());
      rh.SetLeft(cellInfos(lstRow, lstCol - 1).rel_height() - myInfo.rel_height());
      cells(lstRow, lstCol) = YardCell(myInfo, rh);
      rh = 0;
   }

   // Now we can skip all the corners

   // Do first row
   for (size_t col = 1; col < lstCol; ++col) {
      myInfo = cellInfos(0, col);

      rh.SetLeft(cellInfos(0, col - 1).rel_height() - myInfo.rel_height());
      rh.SetBottomLeft(cellInfos(1, col - 1).rel_height() - myInfo.rel_height());
      rh.SetBottom(cellInfos(1, col).rel_height() - myInfo.rel_height());
      rh.SetBottomRight(cellInfos(1, col + 1).rel_height() - myInfo.rel_height());
      rh.SetRight(cellInfos(0, col + 1).rel_height() - myInfo.rel_height());

      cells(0, col) = YardCell(myInfo, rh);
      rh = 0;
   }

   // Do First Column
   for (size_t row = 1; row < lstRow; ++row) {
      myInfo = cellInfos(row, 0);

      rh.SetBottom(cellInfos(row + 1, 0).rel_height() - myInfo.rel_height());
      rh.SetBottomRight(cellInfos(row + 1, 1).rel_height() - myInfo.rel_height());
      rh.SetRight(cellInfos(row, 1).rel_height() - myInfo.rel_height());
      rh.SetTopRight(cellInfos(row - 1, 1).rel_height() - myInfo.rel_height());
      rh.SetTop(cellInfos(row - 1, 0).rel_height() - myInfo.rel_height());

      cells(row, 0) = YardCell(myInfo, rh);
      rh = 0;
   }

   // Do middle chunk
   for (size_t row = 1; row < lstRow; ++row) {
      for (size_t col = 1; col < lstCol; ++col) {
         myInfo = cellInfos(row, col);

         rh.SetRight(cellInfos(row, col+1).rel_height() - myInfo.rel_height());
         rh.SetTopRight(cellInfos(row - 1, col + 1).rel_height() - myInfo.rel_height());
         rh.SetTop(cellInfos(row - 1, col).rel_height() - myInfo.rel_height());
         rh.SetTopLeft(cellInfos(row - 1, col - 1).rel_height() - myInfo.rel_height());
         rh.SetLeft(cellInfos(row, col - 1).rel_height() - myInfo.rel_height());
         rh.SetBottomLeft(cellInfos(row + 1, col - 1).rel_height() - myInfo.rel_height());
         rh.SetBottom(cellInfos(row + 1, col).rel_height() - myInfo.rel_height());
         rh.SetBottomRight(cellInfos(row + 1, col + 1).rel_height() - myInfo.rel_height());

         cells(row, col) = YardCell(myInfo, rh);
         rh = 0;
      }
   }
  
   // Do Last Column
   for (size_t row = 1; row < lstRow; ++row) {
      myInfo = cellInfos(row, lstCol);

      rh.SetTop(cellInfos(row - 1, lstCol).rel_height() - myInfo.rel_height());
      rh.SetTopLeft(cellInfos(row - 1, lstCol - 1).rel_height() - myInfo.rel_height());
      rh.SetLeft(cellInfos(row, lstCol - 1).rel_height() - myInfo.rel_height());
      rh.SetBottomLeft(cellInfos(row + 1, lstCol - 1).rel_height() - myInfo.rel_height());
      rh.SetBottom(cellInfos(row + 1, lstCol).rel_height() - myInfo.rel_height());

      cells(row, lstCol) = YardCell(myInfo, rh);
      rh = 0;
   }

   // Do last row
   for (size_t col = 1; col < cellInfos.size2() - 1; ++col) {
      myInfo = cellInfos(lstRow, col);

      rh.SetLeft(cellInfos(lstRow, col - 1).rel_height() - myInfo.rel_height());
      rh.SetTopLeft(cellInfos(lstRow - 1, col - 1).rel_height() - myInfo.rel_height());
      rh.SetTop(cellInfos(lstRow - 1, col).rel_height() - myInfo.rel_height());
      rh.SetTopRight(cellInfos(lstRow - 1, col + 1).rel_height() - myInfo.rel_height());
      rh.SetRight(cellInfos(lstRow, col + 1).rel_height() - myInfo.rel_height());

      cells(lstRow, col) = YardCell(myInfo, rh);
      rh = 0;
   }

   return cells;
}


const bm::unbounded_array<size_t> Yard::InitHeightMap(const bm::matrix<YardCell> &cells) {
   bm::unbounded_array<size_t> retVal(cells.data().size(), 0);

   // Sort the drift map by height
   auto heightLambda = [&cells](size_t lPos, size_t rPos) {
      double diff =  cells.data()[lPos].cell_info().rel_height() -
                     cells.data()[rPos].cell_info().rel_height();
      return (diff < 0? floor(diff): ceil(diff));
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


void Yard::ElapseTime(pt::time_period tickPeriod, const WeatherData &data, std::vector<pt::time_duration> sprinklerDurations) {
   // TODO: Change the api so that sprinklerDurations cannot be accidentally resized

   cout << "ElapseTime: " << tickPeriod << endl;

   double dt = tickPeriod.length().ticks()/((double)tickPeriod.length().ticks_per_second());

   // Add sprinkler water to the surface
   for (int i = 0; i < sprinkler_masks_.size(); ++i) {
      pt::time_duration dt_s = sprinklerDurations[i];
      surface_water_ += ((dt_s.ticks()/((double)dt_s.ticks_per_second())) * sprinkler_masks_[i]);
   }

   // On every Tick, we need to deliver the right amount of water to each cell
   //std::for_each(make_zip_iterator(boost::make_tuple(sprinklerDurations.begin(), sprinkler_masks_.begin())),
   //              make_zip_iterator(boost::make_tuple(sprinklerDurations.end(), sprinkler_masks_.end())),
   //              [this](boost::tuple<pt::time_duration, bm::compressed_matrix<double> > p) {
   //   // Tuple consists of (sprinkler durations, mask)
   //   pt::time_duration t = p.get<0>();
   //   bm::compressed_matrix<double> m = p.get<1>();

   //   surface_water_ += ((t.ticks()/((double)t.ticks_per_second())) * m);
   //});

   // Add rain water to the surface
   if (data.rainfall().is_initialized()) {
      surface_water_ += (data.rainfall().get() * rain_mask_);
   }

   
   //Redistribute the water over the surface
   for (size_t pos : cells_by_height_) {
      DriftEntry de = cells_.data()[pos].drift_entry();
      if (de.Right()) {
         // TODO: Use matrix slices
      }
   }
   
   // Shine sunlight
   
   // Apply heat
   
   // Apply humidity
   
   // Blow wind


   // Grow

}








}
}

