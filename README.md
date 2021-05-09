# Get project

git clone  --recurse-submodules git@github.com:arqubusier/programmable-remote.git

# Build using docker

    $ cd <project-root>

Build docker image like so:

    $ docker build -t stm32-cpp-bare 


Run the container:

    $ docker run -it --name stm32-cpp-bare -p 4444:4444 -v "$(pwd)/app":/usr/src/app --privileged -v /dev/bus/usb:/dev/bus/usb stm32-cpp-bare /bin/bash

If you stop the container, you can start it again with:

    $ docker start -a -i stm32-cpp-bare

The project root directory on the host machine and "/usr/src/app" in the container can be used to share data.

## First-time setup

In the container run:

  root@mycontainer $ make -C libopencm3

## Build

In the container run:

  root@mycontainer $ make -C src

## Flashing

This project is currently only tested for the "bluepill" devboard.
In the container run:

  root@mycontainer $ openocd -f bluepill.cfg -c "program src/programmable-remote.elf verify reset exit"


