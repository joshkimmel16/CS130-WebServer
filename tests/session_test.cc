#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "session.h"

class SessionTest : public ::testing::Test {
  protected:
    char* r;
    size_t r_size;
};

class MockIO : public boost::asio::io_service {
};

TEST_F(SessionTest, CallStart) {
	MockIO service;
	session* sess = new session(service);
	EXPECT_TRUE(sess->start());
}

TEST_F(SessionTest, CallRead) {
	MockIO service;
	session* sess = new session(service);
	EXPECT_TRUE(sess->read());
}

TEST_F(SessionTest, CallWrite) {
	MockIO service;
	session* sess = new session(service);
	int status_code = 400;
  	std::string input = "GET /test HTTP/1.1\r\n\r\n";
	int n = input.length();    
	char char_array[n+1];   
	strcpy(char_array, input.c_str());
	r = char_array;
	r_size = std::strlen(char_array);

	request* req = new request(r, r_size);

	EXPECT_TRUE(sess->write(req, status_code));
}