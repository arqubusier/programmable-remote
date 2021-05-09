# Build using docker

Build docker image like so:

    $ docker build -t stm32-cpp-bare 


Run a container:

    $ docker run -it --name stm32-cpp-bare -p 4444:4444 -v "$(pwd)/app":/usr/src/app --privileged -v /dev/bus/usb:/dev/bus/usb stm32-cpp-bare /bin/bash

The project root directory on the host machine and "/usr/src/app" in the container can be used to share data.

## Flashing

In the container run:

    $ openocd -f bluepill.cfg -c "program exe.elf verify reset exit"


