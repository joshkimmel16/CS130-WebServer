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

using boost::asio::ip::tcp;

int main(int argc, char* argv[]) 
{
  //testing to see Boost log works///////////////////////////////
  logger l();
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
