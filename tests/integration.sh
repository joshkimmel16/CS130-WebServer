#!/bin/bash

#function to print proper usage for the bash script
print_usage() {
  echo "Valid commands are: -h (help), -c PATH_TO_SERVER_CONFIG, -b (execute invalid request)"
  exit 1
}

#variable declarations with default values
TEST_DIR=$(pwd)
SERVER_PATH="$(pwd)/../build/bin/server_main"
CONFIG="integration_config"
REQ_TYPE=0
SERVER_PID=

#capture arguments to the script
while getopts "hc:b" opt; do
  case $opt in
    h)
      print_usage
      ;;
    c)
      CONFIG=$OPTARG
      ;;
    b)
      REQ_TYPE=1
      ;;
    \?)
      echo "Invalid option (-$OPTARG)"
      print_usage
      ;;
  esac
done

SERVER_EX="${SERVER_PATH} ${CONFIG}"

#run the server and capture its PID
$SERVER_EX &
SERVER_PID=$!

#check to ensure the server is running
if [ ! -z "$SERVER_PID" ]
then
    echo "Server is running!"
    #run test request
    #check result
    #kill server process
    kill $SERVER_PID
else
    echo "The web server failed to start!"
    exit 2
fi

echo "We made it!"
exit 0