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
	}

	using namespace DataSetOptionalInternal;

	template<class data_t, class data_enum_t, size_t enum_count = static_cast<size_t>(data_enum_t::ENUM_COUNT)>
	class DataSetOptional
	{
	public:
		DataSetOptional()
		{
			size_t i = 0;

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
