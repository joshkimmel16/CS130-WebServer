// boost logger

#include "logger.h"

logger::logger() {
  init_log();
}

//Code from :
//https://blog.scalyr.com/2018/07/getting-started-quickly-c-logging/
//
//init_log is for testing only (for boost log and output file)
void logger::init_log()
{
  static const std::string COMMON_FMT("[%TimeStamp%][%Severity%]:  %Message%");
  boost::log::register_simple_formatter_factory< boost::log::trivial::severity_level, char >("Severity");
  boost::log::add_console_log
  (
      std::cout,
      boost::log::keywords::format = COMMON_FMT,
      boost::log::keywords::auto_flush = true
  );


  boost::log::add_file_log (
      boost::log::keywords::file_name = "sample_%3N.log",
      boost::log::keywords::rotation_size = 1 * 1024 * 1024,
      boost::log::keywords::max_size = 20 * 1024 * 1024,
      boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
      boost::log::keywords::format = COMMON_FMT,
      boost::log::keywords::auto_flush = true
  );

  boost::log::add_common_attributes();

#ifndef _DEBUG
  boost::log::core::get()->set_filter(
      boost::log::trivial::severity >= boost::log::trivial::info
  );
#endif

}