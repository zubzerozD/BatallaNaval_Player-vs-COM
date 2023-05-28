CC=gcc
CXX=g++
RM=rm -f
CXXFLAGS= -lstdc++ -pthread

.PHONY: all clean

all: server client

server: server
	$(CC) -o server server.cpp $(CXXFLAGS)

client: client
	$(CC) -o cliente client.cpp $(CXXFLAGS)

clean:
	$(RM) server client