cmake_minimum_required(VERSION 3.10.0)
project(gitty-up-web-server)

# Make sure developers do not run cmake in the main project directory, to keep
# build artifacts from becoming clutter
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed.
    Please make a new directory (called a build directory) and run CMake from there.
    You may need to remove CMakeCache.txt." )
endif()

# Turn on debug builds if we are building in a devel shell
if (CMAKE_BUILD_TYPE STREQUAL "" AND "$ENV{DEVEL_SHELL}" STREQUAL "1")
    message(STATUS "Setting debug build type by default in devel shell")
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Output binaries to a sub directory "bin"
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable cmake testing
include(CTest)
enable_testing()

# Enable GoogleTest
include(GoogleTest)
add_subdirectory(/usr/src/googletest googletest)

# Enable Boost
# Use static libraries so binaries can be deployed without a full boost install
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.50 REQUIRED COMPONENTS system)
message(STATUS "Boost version: ${Boost_VERSION}")

#Enable Boost Log
find_package(Boost 1.54.0 REQUIRED COMPONENTS log_setup log)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})


include_directories(include)

# TODO(!): Update name and srcs
add_library(server src/server.cc)
add_library(session src/session.cc)
add_library(config_parser src/config_parser.cc)
add_library(request src/request.cc)
add_library(response src/response.cc)
add_library(echo_handler src/echo_handler.cc)
add_library(static_file_handler src/static_file_handler.cc)
add_library(default_handler src/default_handler.cc)
add_library(router src/router.cc)
add_library(logger src/logger.cc)

# TODO(!): Update executable name, srcs, and deps
add_executable(server_main src/server_main.cc)
target_link_libraries(server_main server session config_parser request response logger router echo_handler static_file_handler default_handler Boost::system ${Boost_LIBRARIES})
#ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
#include_directories(${Boost_INCLUDE_DIRS})
#TARGET_LINK_LIBRARIES(server_main ${Boost_LIBRARIES})

# TODO(!): Update test executable name, srcs, and deps
# add_executable(example_lib_test tests/example_lib_test.cc)
# target_link_libraries(example_lib_test example_lib gtest_main)
add_executable(config_parser_test tests/config_parser_test.cc)
target_link_libraries(config_parser_test config_parser gtest_main)
add_executable(request_test tests/request_test.cc)
target_link_libraries(request_test request gtest_main)
add_executable(response_test tests/response_test.cc)
target_link_libraries(response_test response gtest_main)
add_executable(session_test tests/session_test.cc)
target_link_libraries(session_test session request response config_parser router echo_handler static_file_handler default_handler gtest_main Boost::system)
add_executable(server_test tests/server_test.cc)
target_link_libraries(server_test server session request response config_parser router echo_handler static_file_handler default_handler gtest_main Boost::system)
add_executable(echo_handler_test tests/echo_handler_test.cc)
target_link_libraries(echo_handler_test echo_handler request response config_parser gtest_main)
add_executable(static_file_handler_test tests/static_file_handler_test.cc)
target_link_libraries(static_file_handler_test static_file_handler request response config_parser gtest_main)
add_executable(default_handler_test tests/default_handler_test.cc)
target_link_libraries(default_handler_test default_handler request response config_parser gtest_main)
add_executable(router_test tests/router_test.cc)
target_link_libraries(router_test router echo_handler static_file_handler default_handler request response config_parser gtest_main)



# TODO(!): Update with test binary
# gtest_discover_tests(example_lib_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(config_parser_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/config)
gtest_discover_tests(request_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(response_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(session_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(server_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(echo_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(static_file_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(default_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(router_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# Integration test for simple GET request with no body or extra headers
add_test(NAME IntegrationTest.SimpleGet
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration.sh -c Integration/config_8080 -p 8080 -m GET -u /echo -o Integration/simple_get_output
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    
# Integration test for handling of invalid HTTP request
add_test(NAME IntegrationTest.InvalidHTTP
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration.sh -c Integration/config_8080 -p 8080 -n -b blahblahblah -o Integration/invalid_http_output
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# TODO(!): Update with target/test targets
# include(cmake/CodeCoverageReportConfig.cmake)
# generate_coverage_report(TARGETS example_server example_lib TESTS example_lib_test)
include(cmake/CodeCoverageReportConfig.cmake)
generate_coverage_report(TARGETS server_main server session config_parser request response router echo_handler static_file_handler default_handler logger TESTS config_parser_test request_test response_test session_test server_test router_test echo_handler_test static_file_handler_test default_handler_test)
