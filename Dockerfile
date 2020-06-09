FROM ubuntu:18.04

RUN apt update && apt install -y\
    cmake \
    g++ \
    libncurses5-dev\
    libncursesw5-dev\
    make

COPY ./ /usr/src/CPPND-SYSTEM-MONITOR

WORKDIR /usr/src/CPPND-SYSTEM-MONITOR
RUN make build

CMD [ "bash" ]
