#pragma once

#include "LawnSimStd.h"

namespace Mist { namespace LawnSim { namespace Exceptions {


class InvalidOperationException : public std::logic_error
{
public:
   InvalidOperationException(const std::string& what_arg)
      : std::logic_error(what_arg)
   {
   }

   InvalidOperationException(const char * const what_arg)
      : std::logic_error(what_arg)
   {
   }

   InvalidOperationException()
      : std::logic_error("Invalid Operation")
   {
   }
};


}}}