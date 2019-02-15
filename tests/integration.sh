#!/bin/bash

#function to print proper usage for the bash script
print_usage() {
  STR=$'Valid commands are: 
  -h Help. 
  -c Path to server config file. Default is: "Integration/integration_config". 
  -n Use netcat (nc) instead of cURL 
  -p Port that the web server will run on. Default is: 8080. 
  -t Host that the request should be made to. Default is: localhost. 
  -u URI of the request (cURL only). / by default. 
  -m Request method (cURL only). Default is: "GET" 
  -d Header for the request (cURL only). This option can be used multiple times (per header to be added). 
  -b Request body (cURL) OR entire request (netcat). Empty by default. 
  -o Path to a file containing the expected response for the given test. Default is: Integration/expected_output.'
  echo "$STR"
  exit 1
}

create_header_string() {
    if [ ! ${#REQ_HEADERS[@]} -eq 0 ]
    then
        for header in "${REQ_HEADERS[@]}"; do
            if [ ! -z "$header" ]
            then
                REQ_HEADER_STRING+=" -H ${header} "
            fi
        done
    fi
}

create_curl_request() {
    create_header_string
    if [ ! -z "$REQ_BODY" ]
    then
        REQ_COMMAND="curl -i -s -X ${REQ_METHOD}${REQ_HEADER_STRING} --data \"${REQ_BODY}\" http://${REQ_HOST}:${SERVER_PORT}${REQ_URI}"
    else
        REQ_COMMAND="curl -i -s -X ${REQ_METHOD}${REQ_HEADER_STRING} http://${REQ_HOST}:${SERVER_PORT}${REQ_URI}"
    fi
}

create_nc_request() {
    REQ_COMMAND="nc ${REQ_HOST} ${SERVER_PORT}"
}

#variable declarations with default values
TEST_DIR=$(pwd)
CONFIG="Integration/integration_config"
USE_NETCAT="0"

REQ_COMMAND=""

REQ_HOST="localhost"
REQ_URI=
REQ_METHOD="GET"
REQ_HEADERS=
REQ_HEADER_STRING=""
REQ_BODY=

SERVER_PATH="$(pwd)/../build/bin/server_main"
SERVER_PID=
SERVER_PORT="8080"

EXPECTED_OUTPUT="Integration/expected_output"

#capture arguments to the script
while getopts "hc:np:t:u:m:d:b:o:" opt; do
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
    t)
      REQ_HOST=$OPTARG
      ;;
    u)
      REQ_URI=$OPTARG
      ;;
    m)
      REQ_METHOD=$OPTARG
      ;;
    d)
      REQ_HEADERS+=($OPTARG)
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
    #give the server some time to start up
    sleep 0.3
    
    #generate the appropriate request
    #then execute the generated request
    if [ "$USE_NETCAT" == "1" ]
    then
        create_nc_request
        OUTPUT=$(echo $REQ_BODY | $REQ_COMMAND)
        echo "$OUTPUT" &> "Integration/temp"
    else
        create_curl_request
        $($REQ_COMMAND &> Integration/temp)
    fi
    
    #diff the output with some expected result
    VERIFY=$(grep -Fwf $EXPECTED_OUTPUT Integration/temp)
    if [ -z "$VERIFY" ]
    then
        echo "The results don't match!"
        kill $SERVER_PID
        exit 1
    fi
    
    #kill server process
    kill $SERVER_PID
else
    echo "The web server failed to start!"
    exit 1
fi

exit 0