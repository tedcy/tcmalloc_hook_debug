include ../tcmalloc.mk
.PHONY: PFishHook
PFishHook_LIB := ./PFishHook/build/libPFishHook.a
BUILD_PFishHook = cd PFishHook && mkdir -p build && cd build && cmake .. && make -j

all: libdynamic.so ${PFishHook_LIB} ${TCM_LIB}
	g++ -std=c++14 -g -fno-omit-frame-pointer -o main hook.cpp main.cpp \
    ${PFishHook_LIB} ./PFishHook/build/3rdparty/zydis/libZydis.a \
	${TCM_LIB} -Wl,-Bstatic -lunwind -Wl,-Bdynamic \
	-lpthread -ldl
	
libdynamic.so: dynamic_lib.cpp
	g++ -g -shared -fPIC -o libdynamic.so dynamic_lib.cpp

PFishHook: 
	${BUILD_PFishHook}

${PFishHook_LIB}:
	${BUILD_PFishHook}

clean:
	rm -f main libdynamic.so
