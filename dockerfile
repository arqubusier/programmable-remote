FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update -q
#install software requirements
RUN apt-get install --no-install-recommends -y software-properties-common build-essential git symlinks expect

# Install build dependancies
RUN apt-get install -y binutils-arm-none-eabi \
gcc-arm-none-eabi \
gdb-multiarch \
libnewlib-arm-none-eabi \
libstdc++-arm-none-eabi-newlib \
openocd \
python \
cmake \
bear \
tmux \
telnet

#OpenOCD talks to the chip through USB, so we need to grant our account access to the FTDI.
##RUN cp /usr/local/share/openocd/contrib/60-openocd.rules /etc/udev/rules.d/60-openocd.rules

#create a directory for our project & setup a shared workfolder between the host and docker container
RUN mkdir -p /usr/src/app
VOLUME ["/usr/src/app"]
WORKDIR /usr/src/app
RUN cd /usr/src/app

EXPOSE 4444
