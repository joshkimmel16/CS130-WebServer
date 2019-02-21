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

//run the new session in a thread from the thread pool
void session_thread(int thread_id, session* new_session)
{
    new_session->start();
}

//constructor takes a Boost io_service object, an NginxConfig object, and a port
//and executes the start_accept private method
server::server(boost::asio::io_service& io_service, NginxConfig config, short port, unsigned int threads)
: io_service_(io_service),
  config_(config),
  isRunning(false),
  pool_(threads),
  acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
{}

//start_accept establishes and blank session binds incoming
//requests to the blank session
bool server::start_accept() 
{
    if (router_ != nullptr)
    {
        session* new_session = new session(io_service_, router_);
        acceptor_.async_accept(new_session->socket(),
            boost::bind(&server::handle_accept, this, new_session,
            boost::asio::placeholders::error));
        isRunning = true;
        return true;
    }
    else
    {
        isRunning = false;
        return false;
    }
}

bool server::get_status() {
  return isRunning;
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
        pool_.push(session_thread, new_session);
    }
    else
    {
        delete new_session;
    }

    start_accept();
}

//create a router for the server to use
bool server::create_router (std::string server_root)
{
    std::shared_ptr<NginxConfig> output = nullptr;
    for (const auto& statement : config_.statements_) 
    {
        //the config we're looking for is nested
        if (statement->child_block_)
        {
            //must have at least one element if the child block is not null
            if (statement->tokens_[0] == "router")
            {
                output = statement->child_block_;
            }
        }
    }
    
    if (output != nullptr)
    {
        std::shared_ptr<router> tmp(new router(output, server_root));
        router_ = tmp;
        router_->register_routes_from_config();
    }
    return (output != nullptr) ? true : false;
}

//register a route in the router
bool server::register_route(std::string uri, std::string route_handler)
{
    return router_->register_route(uri, route_handler);
}

//register a default header in the router
bool server::register_default_header(std::string header_name, std::string header_value)
{
    return router_->register_default_header(header_name, header_value);
}

//method to terminate all active/pending sessions in the thread pool
//by convention, we are forcing all currently running threads to terminate immediately
bool server::stop_all_sessions ()
{
    pool_.stop(false);
    return true;
}