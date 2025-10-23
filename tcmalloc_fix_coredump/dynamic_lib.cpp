#include <cstdlib>

extern "C" void freeMemory(void* ptr) {
    free(ptr);
}