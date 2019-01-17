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

//constructor takes a Boost io_service object and port number
//and executes the start_accept private method
server::server(boost::asio::io_service& io_service, short port)
: io_service_(io_service),
  acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) 
{
      start_accept();
}

//start_accept establishes and blank session binds incoming
//requests to the blank session
void server::start_accept() 
{
    session* new_session = new session(io_service_);
    acceptor_.async_accept(new_session->socket(),
        boost::bind(&server::handle_accept, this, new_session,
          boost::asio::placeholders::error));
}

//handle_accept is the callback executed when start_accept returns
//if successful, the new session is started
//if failed, the blank session is deleted
//regardless, re-execute start_accept to handle subsequent incoming connections
void server::handle_accept(session* new_session,
  const boost::system::error_code& error) 
{
    if (!error) 
    {
      new_session->start();
    }
    else 
    {
      delete new_session;
    }

    start_accept();
}