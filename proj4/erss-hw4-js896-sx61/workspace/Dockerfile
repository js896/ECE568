FROM ubuntu:18.04
RUN mkdir /var/log/erss
RUN apt-get update && apt-get -y install g++ make
RUN apt-get install -y libboost-all-dev

ADD ./workspace /var/log/erss

WORKDIR /var/log/erss/workspace


