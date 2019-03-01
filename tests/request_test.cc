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
    
  std::unique_ptr<request> req(new request(r, r_size));
  EXPECT_EQ(req->get_method(), "GET");
  EXPECT_EQ(req->get_uri(), "/test");
  EXPECT_TRUE(req->is_valid());
  //EXPECT_TRUE(strcmp(req->get_raw_request(), r));
  EXPECT_EQ(req->get_request_size(), r_size);
}

//valid HTTP request with a request line and headers but no body
TEST_F(RequestTest, RequestLineAndHeaders) {
  std::string input = "DELETE /testing HTTP/1.1\r\nAccept: application/json\r\nUser-Agent: Chrome\r\n\r\n";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
    
  std::unique_ptr<request> req(new request(r, r_size));
  EXPECT_EQ(req->get_method(), "DELETE");
  EXPECT_EQ(req->get_uri(), "/testing");
  EXPECT_EQ(req->get_header("Accept"), "application/json");
  EXPECT_EQ(req->get_header("User-Agent"), "Chrome");
  EXPECT_TRUE(req->is_valid());
  //EXPECT_TRUE(strcmp(req->get_raw_request(), r));
  EXPECT_EQ(req->get_request_size(), r_size);
}

//valid HTTP request with a request line and a body but no headers
TEST_F(RequestTest, RequestLineAndBody) {
  std::string input = "POST / HTTP/1.1\r\n\r\nblahblahblah";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
    
  std::unique_ptr<request> req(new request(r, r_size));
  EXPECT_EQ(req->get_method(), "POST");
  EXPECT_EQ(req->get_uri(), "/");
  EXPECT_EQ(req->get_body(), "blahblahblah");
  EXPECT_TRUE(req->is_valid());
  //EXPECT_TRUE(strcmp(req->get_raw_request(), r));
  EXPECT_EQ(req->get_request_size(), r_size);
}

//valid HTTP request with a request line, headers, and a body
TEST_F(RequestTest, FullRequest) {
  std::string input = "PUT /test/another HTTP/1.1\r\nContent-Type: application/json\r\nAuthorization: password\r\n\r\nblahblahblah";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
    
  std::unique_ptr<request> req(new request(r, r_size));
  EXPECT_EQ(req->get_method(), "PUT");
  EXPECT_EQ(req->get_uri(), "/test/another");
  EXPECT_EQ(req->get_header("Content-Type"), "application/json");
  EXPECT_EQ(req->get_header("Authorization"), "password");
  EXPECT_EQ(req->get_header("Random"), "");
  EXPECT_EQ(req->get_body(), "blahblahblah");
  EXPECT_TRUE(req->is_valid());
  //EXPECT_TRUE(strcmp(req->get_raw_request(), r));
  EXPECT_EQ(req->get_request_size(), r_size);
}

//totally garbage request
TEST_F(RequestTest, GarbageRequest) {
  std::string input = "dbnjksnfjksnjkfnsjkjnfjk";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
    
  std::unique_ptr<request> req(new request(r, r_size));
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
    
  std::unique_ptr<request> req(new request(r, r_size));
  EXPECT_FALSE(req->is_valid());
}


TEST_F(RequestTest, EmptyRequest){
  std::string input = "";
  int n = input.length();
  char char_array[n+1];
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);

  std::unique_ptr<request> req(new request(r, r_size));
  EXPECT_FALSE(req->is_valid());
}

TEST_F(RequestTest, QueryStringRequest){
  std::string input = "GET /test?p=1&q=2 HTTP/1.1\r\n\r\n";
  int n = input.length();
  char char_array[n+1];
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);

  std::unique_ptr<request> req(new request(r, r_size));
  EXPECT_TRUE(req->is_valid());
  EXPECT_EQ(req->get_uri(), "/test");
  EXPECT_EQ(req->get_query_string_param("p"), "1");
  EXPECT_EQ(req->get_query_string_param("q"), "2");
  EXPECT_EQ(req->get_query_string_param("random"), "");
}