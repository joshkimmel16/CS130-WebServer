#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "session.h"

class MockIO : public boost::asio::io_service {
};

class SessionTest : public ::testing::Test {
  protected:
    char* r;
    size_t r_size;
    MockIO service;
};

TEST_F(SessionTest, CallStart) {
	session* sess = new session(service);
	EXPECT_TRUE(sess->start());
}

TEST_F(SessionTest, CallRead) {
	session* sess = new session(service);
	EXPECT_TRUE(sess->read());
}

TEST_F(SessionTest, CallWrite) {
	session* sess = new session(service);
    std::string tmp = "hello";
	EXPECT_TRUE(sess->write(tmp.c_str()));
}


TEST_F(SessionTest, HandleSuccessValid) {
	session* sess = new session(service);
    sess->set_data("GET /test HTTP/1.1\r\n\r\n");
    size_t bytes_transferred = 30;
	EXPECT_TRUE(sess->handle_success(bytes_transferred));
}

TEST_F(SessionTest, HandleSuccessInValid) {
	session* sess = new session(service);
    sess->set_data("blahblahblah");
    size_t bytes_transferred = 12;
	EXPECT_FALSE(sess->handle_success(bytes_transferred));
}

TEST_F(SessionTest, HandleError) {
	session* sess = new session(service);
    const boost::system::error_code error;
        
	EXPECT_TRUE(sess->handle_error(error));
}