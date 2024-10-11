FROM debian:latest

RUN apt-get update && \
    apt-get install -y gcc g++ make cmake valgrind && \
    apt-get clean

WORKDIR /usr/src/app
COPY . .

WORKDIR /usr/src/app/build

RUN cmake ..
RUN make -C .

CMD ["/bin/bash"]
