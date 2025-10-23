MAIN_DIR := $(shell git rev-parse --show-toplevel)

all: libdynamic.so
	g++ -std=c++14 -g -o main main.cpp hook.cpp -ltcmalloc -lpthread -ldl

libdynamic.so: dynamic_lib.cpp
	g++ -g -shared -fPIC -o libdynamic.so dynamic_lib.cpp

clean:
	rm -f main libdynamic.so