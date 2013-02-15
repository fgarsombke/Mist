#pragma once

#include "LawnSimStd.h"


namespace Mist { namespace LawnSim {

template<class T>
class NeighborData {
public:
   typedef T value_t;
   typedef bm::c_matrix<value_t, 3, 3> data_t;
   typedef data_t iterator;
   typedef data_t const_iterator;

   // Allow NeighborData to be assigned from a constant
   NeighborData& operator=(value_t value) {
      for (int i = 0; i < 9; ++i) {
         data_[i] = value;
      }

      return *this;
   }

   data_t &data() { return data_; }

   iterator begin() { return data_.data().begin(); }
   iterator end() { return data_ + 9; }

   const_iterator begin() const { return data_; }
   const_iterator end() const { return data_ + 9; }

   value_t Right() const { return data_(1,2); }
   value_t TopRight() const { return data_(0,2); }
   value_t Top() const { return data_(0,1); }
   value_t TopLeft() const { return data_(0,0); }
   value_t Left() const { return data_(1,0); }
   value_t BottomLeft() const { return data_(2,0); }
   value_t Bottom() const { return data_(2,1); }
   value_t BottomRight() const { return data_(2,2); }
   value_t Self() const { return data_(1,1); }

   void SetRight(value_t value) { data_(1,2) = value; }
   void SetTopRight(value_t value) { data_[1] = value; }
   void SetTop(value_t value) { data_[2] = value; }
   void SetTopLeft(value_t value) { data_[3] = value; }
   void SetLeft(value_t value) { data_[4] = value; }
   void SetBottomLeft(value_t value) { data_[5] = value; }
   void SetBottom(value_t value) { data_[6] = value; }
   void SetBottomRight(value_t value) { data_[7] = value; }
   void SetSelf(value_t value) { data_[8] = value; }


private:
   // Drift constants starting at the right going counter clockwise
   bm::c_matrix<value_t, 3, 3> data_;
};


typedef NeighborData<double> NeighborHeightDiffs_t;











}}