#include <cstdlib>
#include <cstring>
#include "server.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

class ServerTest : public ::testing::Test {
  protected:
    short port;
};

class MockIO : public boost::asio::io_service {
};

TEST_F(ServerTest, CallStartAccept) {
	MockIO service;
	port = 8080;
	server* serv = new server(service, port);
	EXPECT_TRUE(serv->get_status());
}