FROM ubuntu:latest

RUN mkdir /workspace
WORKDIR /workspace

RUN apt-get -y update && \
    apt-get -y install \
      --no-install-recommends \
      build-essential \
      gdb \
      man