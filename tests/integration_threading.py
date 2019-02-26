import httplib
import subprocess
import sys
import threading
import time

#function to execute inside of a thread
#makes an HTTP request and collects the response
def MakeRequest(options):
    conn = httplib.HTTPConnection(options["host"])
    response = None
    if options["withData"] is True:
        conn.request(options["method"], options["route"], options["params"], options["headers"])
    else:
        conn.request(options["method"], options["route"])
    response = conn.getresponse()
    data = response.read()
    conn.close()
    output.append(data)

#set up variables
output = []

#these variables are hardcoded since this is a very specific threading test
host = "localhost:8080"
route = "/echo"
method = "GET"
params = None
headers = {"Thread-Sleep": "5"}
target = "Thread-Sleep: 5"

options_data = {"host": host, "route": route, "method": method, "params": params, "headers": headers, "withData": True}
options_no_data = {"host": host, "route": route, "method": method, "withData": False}

#define threads
t1 = threading.Thread(target=MakeRequest, args=(options_data,))
t2 = threading.Thread(target=MakeRequest, args=(options_no_data,))

#run web server
#these arguments are hardcoded since this is a very specific threading test
p = subprocess.Popen(["../build/bin/server_main", "Integration/config_8080"])

# give the server some time to start up
time.sleep(1)

#start threads
#add small sleep to ensure that t1 request is definitely made before t2 request
t1.start()
time.sleep(1)
t2.start()

#wait for all threads to complete
t1.join()
t2.join()

#kill server process
p.terminate()

#check that output appears in the correct order
if len(output) is 2:
    if target in output[0] and target not in output[1]:
        print "Test failed! The sleeping request should return after the non-sleeping request."
        sys.exit(1)
    else:
        print "Test succeeded!"
        sys.exit(0)
else:
    print "Test failed! Expect exactly 2 responses."
    sys.exit(1)