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

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "session.h"

using boost::asio::ip::tcp;

//constructor takes a Boost io_service object
session::session(boost::asio::io_service& io_service)
: socket_(io_service) {}

//socket returns the underlying socket object for this session
tcp::socket& session::socket () 
{
    return socket_;
}

//start executes an asynchronous read in which the server
//waits on data from the requestor
//TODO: is this actually what's happening here?
void session::start() 
{
socket_.async_read_some(boost::asio::buffer(data_, max_length),
    boost::bind(&session::handle_read, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
}

//handle_read is a callback for when the session reads data from the requestor
//TODO: presumably this is where the "echoing" happens
void session::handle_read(const boost::system::error_code& error,
  size_t bytes_transferred) 
{
    if (!error) 
    { 
      char hdr[max_length] = "HTTP-1.1 200 OK Content-Type:text/plain ";
      size_t hdr_len = 40;
      std::memcpy(&hdr[hdr_len], data_, bytes_transferred);
      boost::asio::async_write(socket_,
          boost::asio::buffer(hdr, 40 + bytes_transferred),
          boost::bind(&session::handle_write, this,
          boost::asio::placeholders::error));
    }
    else 
    {
      delete this;
    }
}

//handle_write is a callback for when the server writes data to the requestor
//re-execute the same method from start()
//TODO: not sure if we need to modify this right now...
void session::handle_write(const boost::system::error_code& error) 
{
    if (!error) 
    {
      socket_.async_read_some(boost::asio::buffer(data_, max_length),
         boost::bind(&session::handle_read, this,
         boost::asio::placeholders::error,
         boost::asio::placeholders::bytes_transferred));
    }
    else 
    {
      delete this;
    }
}
