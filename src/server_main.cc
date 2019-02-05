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

void signal_handler(
    const boost::system::error_code& error,
    int signal_number)
{
  if (!error)
  {
    LOG_INFO << "Server has been terminated.";
    exit(1);
  }
}

int main(int argc, char* argv[]) 
{
  //testing to see Boost log works///////////////////////////////
  // Logger logs;
  // logs.logInfo("server_main has been entered...");
  LOG_INFO << "server_main has been entered..";

  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: async_tcp_echo_server <path to config file>\n";
      // logs.logFatal("Invalid number of arguments...");
      LOG_FATAL << "Invalid number of arguments.";
      return 1;
    }

    // parse config file
    NginxConfigParser config_parser;
    NginxConfig config;
    config_parser.Parse(argv[1], &config);
    
    if (!config.ParseStatements()) {
	    // logs.logFatal("parse failure...");
      LOG_FATAL << "Parse failure.";
      return 1;
    } 
    // logs.logInfo("parse succeed...");
    LOG_INFO << "Parse success.";

    int port = config.GetPort();
    boost::asio::io_service io_service;

    // Construct a signal set registered for process termination.
    boost::asio::signal_set signals(io_service, SIGINT, SIGTERM);

    // Start an asynchronous wait for one of the signals to occur.
    signals.async_wait(signal_handler);

    std::unique_ptr<server> serv(new server(io_service, config, port));
    bool r = serv->create_router();
    if (r)
    {
        serv->register_route("/echo", "echo");
        serv->register_route("/static1/.*", "static1");
        serv->register_route("/static2/.*", "static2");
        
        bool start = serv->start_accept();
        if (!start)
        {
            throw std::runtime_error("Could not start the server! Please ensure that the router was properly initialized.");
        }
    }
    else
    {
        throw std::runtime_error("Could not create router! Please ensure that the config file has a router config in it.");
    }
    LOG_INFO << "server runs successfullly";
    io_service.run();
  }
  catch (std::exception& e)
  {
    // std::cerr << "Exception: " << e.what() << "\n";
    // logs.logError("caught exception object by reference...");
    LOG_ERROR << "Exception: " << e.what();
  }

  return 0;
}
