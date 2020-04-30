Since the test consists multiple variants. It is necessary to change the docker-compose.yml file to run different tests.

Different tests need to revise line 9, for example if you want to test pre_create threading strategy with 2048 buckets, this line should be: 

command: "/var/log/erss/workspace/pre_create 2048"

If you want to test per_request threading strategy with 32 buckets, this line should be:

command: "/var/log/erss/workspace/per_request 32"


If you want to test with different you should modify line 15 of docker-compose.yml.
If you want to run with 1 core, this line should be:

cpuset: "0"

If you want to run with 2 cores, this line should be:

cpuset: 0,1

If you want to run with 4 cores, this line should be:

cpuset: 0,1,2,3


After chaning the file docker-compose.yml to what you want to test with, run the following command under the directory "test_docker":

sudo docker-compose up

To end the current server from running and test another threading strategy, press C+c, modify docker-compose.yml in the way described above and run "sudo docker-compose up" again.

To run the clinet side code go to the directory "test_docker/workspace/client", run the following command:

./client hostname client number bucket size lower upper

For example:

./client 127.0.0.1 500 2048 1 3


When running the client code, the client will show things like the following:

new value of bucket updated: new value in bucket number

And at the end it will show you the throughput.