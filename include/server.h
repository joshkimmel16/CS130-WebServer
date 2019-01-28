// A TCP web server
#include "session.h"
#include "config_parser.h"

using boost::asio::ip::tcp;

class server 
{
public:
  //Methods
  server(boost::asio::io_service& io_service, short port); //constructor
  // bool start_accept(); //accept an incoming connection
  bool get_status();
private:
  //Methods
  void start_accept(); //accept an incoming connection
  void handle_accept(session* new_session, const boost::system::error_code& error); //handle async accept

  //Attributes
  boost::asio::io_service& io_service_;
  tcp::acceptor acceptor_;
  bool isRunning = false;
};