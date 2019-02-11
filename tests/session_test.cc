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
    NginxConfigParser parser;
    NginxConfig out_config;
    std::shared_ptr<NginxConfig> config;
    std::shared_ptr<router> rout;
    
    virtual void SetUp() {
      parser.Parse("session/router_config", &out_config);
      config = out_config.statements_[1]->child_block_;
      std::shared_ptr<router> tmp(new router(config));
      rout = tmp;
    }
};

TEST_F(SessionTest, CallStart) {
	std::unique_ptr<session> sess(new session(service, rout));
	EXPECT_TRUE(sess->start());
}

TEST_F(SessionTest, CallRead) {
	std::unique_ptr<session> sess(new session(service, rout));
	EXPECT_TRUE(sess->read());
}

TEST_F(SessionTest, CallWrite) {
	std::unique_ptr<session> sess(new session(service, rout));
    std::string tmp = "hello";
    std::vector<char> bytes(tmp.begin(), tmp.end());
	EXPECT_TRUE(sess->write(bytes));
}

TEST_F(SessionTest, HandleSuccessValid) {
	std::unique_ptr<session> sess(new session(service, rout));
    sess->set_data("GET /test HTTP/1.1\r\n\r\n");
    size_t bytes_transferred = 30;
	EXPECT_TRUE(sess->handle_success(bytes_transferred));
}

TEST_F(SessionTest, HandleSuccessInValid) {
	std::unique_ptr<session> sess(new session(service, rout));
    sess->set_data("blahblahblah");
    size_t bytes_transferred = 12;
	EXPECT_FALSE(sess->handle_success(bytes_transferred));
}

TEST_F(SessionTest, HandleError) {
	std::unique_ptr<session> sess(new session(service, rout));
    const boost::system::error_code error;
        
	EXPECT_TRUE(sess->handle_error(error));
}