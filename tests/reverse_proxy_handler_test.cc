#include <cstdlib>
#include <cstring>
#include <iostream>
#include "gtest/gtest.h"
#include "echo_handler.h"

class ReverseProxyHandlerTest : public ::testing::Test {
  protected:
    char* r;
    size_t r_size;
    std::shared_ptr<NginxConfig> config;
};

//simple valid HTTP request
TEST_F(ReverseProxyHandlerTest, SimpleGet) {
    // TODO: Chengyao
    // add actual test
    EXPECT_TRUE(true);
}