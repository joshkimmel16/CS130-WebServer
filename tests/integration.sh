#!/bin/bash

#function to print proper usage for the bash script
print_usage() {
  STR=$'Valid commands are:\n-h help\n-c PATH TO SERVER CONFIG\n-n USE NETCAT INSTEAD OF CURL\n-p PORT THAT WEB SERVER IS RUNNING ON\n-m VALID REQUEST METHOD\n-b VALID REQUEST BODY\n-o PATH TO FILE CONTAINING THE EXPECTED OUTPUT FOR THE GIVEN REQUEST'
  echo "$STR"
  exit 1
}

create_curl_request() {
    if [ ! -z "$REQ_BODY" ]
    then
        REQ_COMMAND="curl -i -X ${REQ_METHOD} --data \"${REQ_BODY}\" http://localhost:${SERVER_PORT}"
    else
        REQ_COMMAND="curl -i -X ${REQ_METHOD} http://localhost:${SERVER_PORT}"
    fi
}

create_nc_request() {
    REQ_COMMAND="nc localhost ${SERVER_PORT}"
}

#variable declarations with default values
TEST_DIR=$(pwd)
CONFIG="Integration/integration_config"
USE_NETCAT="0"

REQ_COMMAND=""
REQ_METHOD="GET"
REQ_BODY=

SERVER_PATH="$(pwd)/../build/bin/server_main"
SERVER_PID=
SERVER_PORT="8080"

EXPECTED_OUTPUT="Integration/expected_output"

#capture arguments to the script
while getopts "hc:np:m:b:o:" opt; do
  case $opt in
    h)
      print_usage
      ;;
    c)
      CONFIG=$OPTARG
      ;;
    n)
      USE_NETCAT="1"
      ;;
    p)
      SERVER_PORT=$OPTARG
      ;;
    m)
      REQ_METHOD=$OPTARG
      ;;
    b)
      REQ_BODY=$OPTARG
      ;;
    o)
      EXPECTED_OUTPUT=$OPTARG
      ;;
    \?)
      echo "Invalid option (-$OPTARG)"
      print_usage
      ;;
  esac
done

SERVER_EX="${SERVER_PATH} ${CONFIG}"

#run the server (as a background process) and capture its PID
$SERVER_EX &
SERVER_PID=$!

#check to ensure the server is running
if [ ! -z "$SERVER_PID" ]
then
    echo "Server is running!"
    sleep 0.3
    
    #generate the appropriate request
    #then execute the generated request
    if [ "$USE_NETCAT" == "1" ]
    then
        create_nc_request
        OUTPUT=$(echo $REQ_BODY | $REQ_COMMAND)
        echo "$OUTPUT" &> "Integration/tmp"
    else
        create_curl_request
        $($REQ_COMMAND &> Integration/tmp)
    fi
    
    #diff the output with some expected result
    #TODO: FIGURE THIS OUT
    #DIFF=$(diff Integration/tmp $EXPECTED_OUTPUT)
    
    #kill server process
    #TODO: GETTING ERROR WHEN TRYING TO REMOVE TEMP FILE
    kill $SERVER_PID
else
    echo "The web server failed to start!"
    exit 2
fi

exit 0