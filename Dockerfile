FROM ubuntu:latest
ARG DEBIAN_FRONTEND=noninteractive
RUN sed -i s@/archive.ubuntu.com/@/mirrors.tuna.tsinghua.edu.cn/@g /etc/apt/sources.list
RUN sed -i s@/security.ubuntu.com/@/mirrors.tuna.tsinghua.edu.cn/@g /etc/apt/sources.list
RUN apt clean
RUN apt-get update -y
RUN apt install wget lzip build-essential m4 flex bison cmake git libgmp-dev libeigen3-dev -y
WORKDIR /root
RUN wget https://crypto.stanford.edu/pbc/files/pbc-0.5.14.tar.gz
RUN tar zxf pbc-0.5.14.tar.gz
WORKDIR /root/pbc-0.5.14
RUN ./configure
RUN make
RUN make install
RUN git clone https://github.com/Liluan1/LSSS.git /root/LSSS
WORKDIR /root/LSSS/build 
RUN cmake ..
RUN make
RUN ./main