default: all

all: iterative simple multi

iterative: server-io-multiplexing-skeleton.cpp
	g++ -Wall -g -o server-multiplex server-io-multiplexing-skeleton.cpp

simple: client-simple.cpp
	g++ -Wall -g -o client-simple client-simple.cpp

multi: client-multi.cpp
	g++ -Wall -g -o client-multi client-multi.cpp

clean:
	-rm -f server-iterative client-simple client-multi
	-rm -rf *.dSYM/
