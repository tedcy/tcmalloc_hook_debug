.PHONY: all tcmalloc clean

MAIN_DIR := $(shell git rev-parse --show-toplevel)

all: libdynamic.so ${MAIN_DIR}/gperftools/build/.libs/libtcmalloc_and_profiler.a
	g++ -std=c++14 -g -fno-omit-frame-pointer -o main main.cpp \
	${MAIN_DIR}/gperftools/build/.libs/libtcmalloc_and_profiler.a -Wl,-Bstatic -lunwind -Wl,-Bdynamic \
	-lpthread -ldl
	
libdynamic.so: dynamic_lib.cpp
	g++ -g -shared -fPIC -o libdynamic.so dynamic_lib.cpp

tcmalloc:
	cd ${MAIN_DIR}/gperftools && ./autogen.sh && mkdir -pv build && cd build && \
    ../configure --enable-libunwind --enable-shared=no --with-pic && make -j

clean:
	rm -f main libdynamic.so