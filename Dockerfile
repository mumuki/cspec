FROM ubuntu:18.04

RUN apt-get update && apt-get install -y \
    git \
    build-essential \
    vim

RUN git clone https://github.com/mumuki/cspec.git

WORKDIR cspec 

RUN make && make install
