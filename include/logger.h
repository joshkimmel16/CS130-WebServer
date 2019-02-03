// boost logger
#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <boost/log/trivial.hpp>
#include "boost/log/utility/setup.hpp"

class Logger 
{
public:
  Logger();
  ~Logger();

  void logTrace(std::string message);
  void logInfo(std::string message);
  void logWarning(std::string message);
  void logError(std::string message);
  void logFatal(std::string message);
  void logDebug(std::string message);
private:
  void init_log();

  std::string file_name;
    
    
};

#endif
