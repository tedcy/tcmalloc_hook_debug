MAIN_DIR := $(shell git rev-parse --show-toplevel)

all: libdynamic.so
	g++ -std=c++14 -g -o main main.cpp -L. -ldynamic -ltcmalloc -lpthread -ldl
	patchelf --set-rpath . main

libdynamic.so: dynamic_lib.cpp
	g++ -g -shared -fPIC -o libdynamic.so dynamic_lib.cpp
	cp libdynamic.so libdynamic1.so

clean:
	rm -f main libdynamic.so libdynamic1.so