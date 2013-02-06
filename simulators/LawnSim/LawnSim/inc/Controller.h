#pragma once

#include <boost/date_time.hpp>

class Controller {
public:



   virtual void Initialize(boost::posix_time::ptime time) = 0;
   
   virtual void ElapseTime(unsigned int elapsed_ms) = 0;

};