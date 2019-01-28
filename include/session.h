// A TCP session
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "request.h"
#include "response.h"

using boost::asio::ip::tcp;

#define OK 200
#define INVALID 400

class session 
{
public:
  //Methods
  session(boost::asio::io_service& io_service); //constructor
  tcp::socket& socket(); //get underlying socket object
  bool start(); //start a session
  bool read();
  bool write(request* req, const unsigned int status_code);
private:
  //Methods
  void handle_read(const boost::system::error_code& error, size_t bytes_transferred); //handle async reads
  void handle_write(const boost::system::error_code& error); //handle async writes
  // void read();
  // void write(request* req, const unsigned int status_code);

  //Attributes
  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length]; 
};
