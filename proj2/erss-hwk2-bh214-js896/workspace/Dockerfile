FROM gcc
RUN apt-get -y update && apt-get install -y g++
RUN apt-get -y update && apt-get install -y make
RUN mkdir /workspace
RUN mkdir /var/log/erss
WORKDIR /workspace
ADD . /workspace
