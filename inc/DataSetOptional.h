#pragma once

#include "MistStd.h"

namespace Mist {
	namespace DataSetOptionalInternal {
		template<class data_t>
		struct DataEntry
		{
			data_t Data;
			bool IsSet;
		};

      template<class data_t, class data_enum_t>
      class DataSetConstIterator
      {
      public:
         DataSetConstIterator()
            : curr_(nullptr), end_(nullptr)
         {}

         DataSetConstIterator(const DataEntry<data_t>* start,
                              const DataEntry<data_t>* end)
            :start_(start), curr_(start), end_(end)
         {
            while(!(curr_->IsSet) && curr_<end_) {
               ++curr_;
            }; 
         }

         const std::pair<data_enum_t, data_t> operator *() const
         { return std::make_pair(static_cast<data_enum_t>(curr_-start_), curr_->Data); }

         DataSetConstIterator& operator++()
         { 
            ++curr_;
            while(!(curr_->IsSet) && curr_<end_) {
               ++curr_;
            }; 
            
            return *this;
         }

         DataSetConstIterator operator++(int)
         { 
            DataSetConstIterator ret(*this);
            ++(*this); 
            return ret;
         }
         
         bool operator <(const DataSetConstIterator &other)
         { return curr_<other.curr_; }

         bool operator ==(const DataSetConstIterator &other)
         { return curr_==other.curr_; }

         bool operator <=(const DataSetConstIterator &other)
         { return curr_<=other.curr_; }

         bool operator >=(const DataSetConstIterator &other)
         { return curr_>=other.curr_; }

         bool operator >(const DataSetConstIterator &other)
         { return curr_>other.curr_; }


      private:
         const DataEntry<data_t>* start_;
         const DataEntry<data_t>* curr_;
         const DataEntry<data_t>* end_;
      };
	}

	using namespace DataSetOptionalInternal;

	template<class data_t, class data_enum_t, size_t enum_count = static_cast<size_t>(data_enum_t::ENUM_COUNT)>
	class DataSetOptional
	{
	public:
      typedef DataSetConstIterator<data_t, data_enum_t> const_iterator;


		DataSetOptional()
		{
			for (size_t i=0; i < enum_count; ++i) {
				data_entries_[i].IsSet = false;
			}
		}

		inline data_t GetValue(data_enum_t val) const
		{
			return data_entries_[static_cast<size_t>(val)].Data;
		}

		inline bool IsSet(data_enum_t val) const
		{
			return data_entries_[static_cast<size_t>(val)].IsSet;
		}
		
      const_iterator AllSetValues() const
      {
         return const_iterator(data_entries_, &data_entries_[enum_count]);
      }

      const_iterator end() const { return const_iterator(&data_entries_[enum_count],&data_entries_[enum_count]); }

	protected:
		void SetValue(data_enum_t type, data_t data) 
		{
			// Assign the value and indicate that it has been assigned
			data_entries_[static_cast<size_t>(type)].Data = data;
			data_entries_[static_cast<size_t>(type)].IsSet = true;
		}

	private:
		DataEntry<data_t> data_entries_[enum_count];
	};

}
