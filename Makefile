default: resolve

resolve: lab1-resolve.cpp
	g++ -Wall -g -o resolve lab1-resolve.cpp

clean:
	-rm -f resolve
