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
#include <string>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "session.h"
// #include "response.h"

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
bool session::start() 
{
     return read();
}


bool session::read(){
     socket_.async_read_some(boost::asio::buffer(data_, max_length),
       boost::bind(&session::handle_read, this,
       boost::asio::placeholders::error,
       boost::asio::placeholders::bytes_transferred));

     return true;
}

//handle_read is a callback for when the session reads data from the requestor
//TODO: presumably this is where the "echoing" happens
void session::handle_read(const boost::system::error_code& error,
  size_t bytes_transferred) 
{
    if (!error) 
    { 
      //parse raw request to a request object
      char* d = data_;
      request* req = new request(d, bytes_transferred);
      
      //check request for validity
      if (req->is_valid())
      {
	       write(req, OK);
      }
      else
      {
	       write(req, INVALID);
      }
    }
    else 
    {
      delete this;
    }
}

bool session::write(request* req, const unsigned int status_code){
     response* resp = new response(status_code, std::string(req->get_raw_request()));
     resp->set_header("Content-Type", "text/plain");
     const char* hdr = resp->generate_response();

     boost::asio::async_write(socket_,
        boost::asio::buffer(hdr, std::strlen(hdr)),
        boost::bind(&session::handle_write, this,
        boost::asio::placeholders::error));
    
     delete req;
     delete resp;

     return true;
}

//handle_write is a callback for when the server writes data to the requestor
//re-execute the same method from start()
//TODO: not sure if we need to modify this right now...
void session::handle_write(const boost::system::error_code& error) 
{
    if (!error) 
    {
      // socket_.async_read_some(boost::asio::buffer(data_, max_length),
      //    boost::bind(&session::handle_read, this,
      //    boost::asio::placeholders::error,
      //    boost::asio::placeholders::bytes_transferred));
      socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send);
      socket_.close();
        
      delete this;
    }
    else 
    {
      delete this;
    }
}
