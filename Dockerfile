FROM ubuntu:20.04

RUN apt-get update;\
	apt-get install -y cmake g++;

RUN	apt-get install -y flex bison;

RUN	apt-get install -y git;
RUN	apt-get install -y libboost-all-dev;
RUN	apt-get install -y doxygen;

RUN	mkdir -p /usr/catch2_git/
WORKDIR /usr/catch2_git/
RUN	git clone https://github.com/catchorg/Catch2.git -b v2.x;
WORKDIR	/usr/catch2_git/Catch2/
RUN	cmake -Bbuild -H. -DBUILD_TESTING=OFF;
RUN	cmake --build build/ --target install;


COPY . /usr/src/tchecker3_gta
WORKDIR /usr/src/tchecker3_gta

RUN ./compile.sh

ENTRYPOINT [ "./run.sh" ]
