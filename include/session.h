// A TCP session
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class session 
{
public:
  //Methods
  session(boost::asio::io_service& io_service); //constructor
  tcp::socket& socket(); //get underlying socket object
  void start(); //start a session
private:
  //Methods
  void handle_read(const boost::system::error_code& error, size_t bytes_transferred); //handle async reads
  void handle_write(const boost::system::error_code& error); //handle async writes

  //Attributes
  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];
  char send_out[max_length];
};
