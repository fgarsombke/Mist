#pragma once

#include "LawnSimStd.h"

namespace Mist { namespace LawnSim { namespace Exceptions {


class InvalidOperationException : public std::exception
{
public:
   InvalidOperationException(const char * const &c)
      : std::exception(c)
   {
   }

   InvalidOperationException(const char * &c, int i)
      : std::exception(c, i)
   {
   }

   InvalidOperationException()
      : std::exception()
   {
   }
};


}}}