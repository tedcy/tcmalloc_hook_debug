include ../tcmalloc.mk

all: libdynamic.so ${TCM_LIB}
	g++ -std=c++14 -g -fno-omit-frame-pointer -o main main.cpp \
    ${TCM_LIB} -Wl,-Bstatic -lunwind -Wl,-Bdynamic \
	-lpthread -ldl

libdynamic.so: dynamic_lib.cpp
	g++ -g -shared -fPIC -o libdynamic.so dynamic_lib.cpp

clean:
	rm -f main libdynamic.so