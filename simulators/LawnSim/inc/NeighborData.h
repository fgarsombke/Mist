#pragma once

#include "LawnSimStd.h"


namespace Mist { namespace LawnSim {

template<class T>
class NeighborData {
public:
   typedef T value_t;
   typedef bm::c_matrix<value_t, 3, 3> data_t;
   typedef value_t * iterator;
   typedef const value_t * const_iterator;

   // Allow NeighborData to be assigned from a constant
   NeighborData& operator=(value_t value) {
      for (value_t &val : *this) {
         val = value;
      }

      return *this;
   }

   const data_t &data() const { return data_; }

   iterator begin() { return &data_.data()[0]; }
   iterator end() { return &data_.data()[0] + 9; }

   const_iterator begin() const { return &data_.data()[0]; }
   const_iterator end() const { return &data_.data()[0] + 9; }

   const value_t &Right() const { return data_(1,2); }
   const value_t &TopRight() const { return data_(0,2); }
   const value_t &Top() const { return data_(0,1); }
   const value_t &TopLeft() const { return data_(0,0); }
   const value_t &Left() const { return data_(1,0); }
   const value_t &BottomLeft() const { return data_(2,0); }
   const value_t &Bottom() const { return data_(2,1); }
   const value_t &BottomRight() const { return data_(2,2); }
   const value_t &Center() const { return data_(1,1); }

   value_t &Right()  { return data_(1,2); }
   value_t &TopRight()  { return data_(0,2); }
   value_t &Top()  { return data_(0,1); }
   value_t &TopLeft()  { return data_(0,0); }
   value_t &Left()  { return data_(1,0); }
   value_t &BottomLeft()  { return data_(2,0); }
   value_t &Bottom()  { return data_(2,1); }
   value_t &BottomRight()  { return data_(2,2); }
   value_t &Center()  { return data_(1,1); }
   
protected:
   data_t &data_mutable() { return data_; }

private:
   // Drift constants starting at the right going counter clockwise
   bm::c_matrix<value_t, 3, 3> data_;
};


typedef NeighborData<double> NeighborHeightDiffs_t;











}}