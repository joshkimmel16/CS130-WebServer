//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "server.h"
#include <boost/log/trivial.hpp>
#include "boost/log/utility/setup.hpp"

//Code from :
//https://blog.scalyr.com/2018/07/getting-started-quickly-c-logging/
//
//init_log is for testing only (for boost log and output file)
static void init_log()
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


using boost::asio::ip::tcp;

int main(int argc, char* argv[]) 
{
  //testing to see Boost log works///////////////////////////////
  init_log();
  BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
  BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
  BOOST_LOG_TRIVIAL(info) << "An informational severity message";
  ///////////////////////////////////////////////////////////////
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: async_tcp_echo_server <path to config file>\n";
      BOOST_LOG_TRIVIAL(error);
      return 1;
    }

    // parse config file
    NginxConfigParser config_parser;
    NginxConfig config;
    config_parser.Parse(argv[1], &config);

    if (!config.ParseStatements()) {
      return 1;
    } 
    int port = config.GetPort();
    
    boost::asio::io_service io_service;

    server s(io_service, port);

    io_service.run();
    BOOST_LOG_TRIVIAL(info) << "server runs successfullly";
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
