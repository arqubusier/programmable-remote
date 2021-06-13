# Get project

git clone  --recurse-submodules git@github.com:arqubusier/programmable-remote.git

# Build using docker

    $ cd <project-root>

Build docker image like so:

    $ docker build -t stm32-cpp-bare .


Run the container:

    $ docker run -it --name stm32-cpp-bare -p 4444:4444 -v "$(pwd)":/usr/src/app --privileged -v /dev/bus/usb:/dev/bus/usb stm32-cpp-bare /bin/bash

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

## compile and flash using running openocd server

Start openocd server

  root@mycontainer openocd -f bluepill.cfg
  
Build and flash:

DOES NOT WORK
  root@mycontainer make -C src && echo "program src/programmable-remote.elf verify reset" | telnet localhost 4444

## Debug with openocd and gdb

  root@mycontainer gdb-multiarch -tui --eval-command="target remote localhost:3333" src/programmable-remote.elf

## Generate compile_commands.json

  $ make -C src clean
  $ bear make -C src
