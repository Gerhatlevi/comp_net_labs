default: all

all: server client-simple client-multi

server: server-iterative.cpp
	g++ -Wall -g -o server server-iterative.cpp

client-simple: client-simple.cpp
	g++ -Wall -g -o client-simple client-simple.cpp

client-multi: client-multi.cpp
	g++ -Wall -g -o client-multi client-multi.cpp

clean:
	-rm -f server client-simple client-multi
	-rm -rf *.dSYM/
