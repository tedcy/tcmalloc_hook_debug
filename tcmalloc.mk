.DEFAULT_GOAL := all
.PHONY: tcmalloc
MAIN_DIR := $(shell git rev-parse --show-toplevel)
TCM_LIB := $(MAIN_DIR)/gperftools/build/.libs/libtcmalloc_and_profiler.a
BUILD_TCM = cd ${MAIN_DIR}/gperftools && ./autogen.sh && mkdir -pv build && cd build && \
    ../configure --enable-libunwind --enable-shared=no --with-pic && make -j

tcmalloc: 
	${BUILD_TCM}

$(TCM_LIB):
	${BUILD_TCM}
