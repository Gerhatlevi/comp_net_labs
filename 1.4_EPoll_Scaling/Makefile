default: all

all: simple multi

simple: client-simple.cpp
	g++ -Wall -g -o client-simple client-simple.cpp

multi: client-multi.cpp
	g++ -Wall -g -o client-multi client-multi.cpp

clean:
	-rm -f client-simple client-multi
	-rm -rf *.dSYM/
