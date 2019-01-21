#include <cstdlib>
#include <cstring>
#include <iostream>
#include "gtest/gtest.h"
#include "request.h"

class RequestTest : public ::testing::Test {
  protected:
    char* r;
    size_t r_size;
};

//simple valid HTTP request with only a request line
TEST_F(RequestTest, RequestLineOnly) {
  std::string input = "GET /test HTTP/1.1\r\n\r\n";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
    
  request* req = new request(r, r_size);
  EXPECT_EQ(req->get_method(), "GET");
  EXPECT_TRUE(req->is_valid());
}

//valid HTTP request with a request line and headers but no body
TEST_F(RequestTest, RequestLineAndHeaders) {
  std::string input = "DELETE /test HTTP/1.1\r\nAccept: application/json\r\nUser-Agent: Chrome\r\n\r\n";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
    
  request* req = new request(r, r_size);
  EXPECT_EQ(req->get_method(), "DELETE");
  EXPECT_EQ(req->get_header("Accept"), "application/json");
  EXPECT_EQ(req->get_header("User-Agent"), "Chrome");
  EXPECT_TRUE(req->is_valid());
}

//valid HTTP request with a request line and a body but no headers
TEST_F(RequestTest, RequestLineAndBody) {
  std::string input = "POST /test HTTP/1.1\r\n\r\nblahblahblah";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
    
  request* req = new request(r, r_size);
  EXPECT_EQ(req->get_method(), "POST");
  EXPECT_EQ(req->get_body(), "blahblahblah");
  EXPECT_TRUE(req->is_valid());
}

//valid HTTP request with a request line, headers, and a body
TEST_F(RequestTest, FullRequest) {
  std::string input = "PUT /test HTTP/1.1\r\nContent-Type: application/json\r\nAuthorization: password\r\n\r\nblahblahblah";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
    
  request* req = new request(r, r_size);
  EXPECT_EQ(req->get_method(), "PUT");
  EXPECT_EQ(req->get_header("Content-Type"), "application/json");
  EXPECT_EQ(req->get_header("Authorization"), "password");
  EXPECT_EQ(req->get_body(), "blahblahblah");
  EXPECT_TRUE(req->is_valid());
}

//totally garbage request
TEST_F(RequestTest, GarbageRequest) {
  std::string input = "dbnjksnfjksnjkfnsjkjnfjk";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
    
  request* req = new request(r, r_size);
  EXPECT_FALSE(req->is_valid());
}

//bad request line
TEST_F(RequestTest, BadRequestLine) {
  std::string input = "GET HTTP/1.1\r\n\r\n";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
    
  request* req = new request(r, r_size);
  EXPECT_FALSE(req->is_valid());
}