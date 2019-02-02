// boost logger
#ifndef LOGGER_H
#define LOGGER_H

#include <boost/log/trivial.hpp>
#include "boost/log/utility/setup.hpp"

class logger 
{
public:
  //Methods
  logger(); //constructor
private:
  //Methods
  void init_log();
  
};

#endif