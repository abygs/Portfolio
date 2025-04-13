#
# PROGRAM: Assignment 7
# AUTHOR:  Abigail Shulgan
#

CXXFLAGS = -ansi -pedantic -Wall -Werror -g -std=c++14

all: client server

client: client.o
	g++ $(CXXFLAGS) -o client client.o

client.o: client.cpp
	g++ $(CXXFLAGS) -c -o client.o client.cpp

server: server.o
	g++ $(CXXFLAGS) -o server server.o

server.o: server.cpp
	g++ $(CXXFLAGS) -c -o server.o server.cpp

clean:
	rm -f *.o client server
