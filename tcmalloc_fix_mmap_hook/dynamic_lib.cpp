#include <cstdlib>
#include <sys/mman.h>

extern "C" void* test() {
    void* p = new char[4 * 1024 * 1024];
    return mmap(NULL, 4 * 1024 * 1024, PROT_READ | PROT_WRITE,
                      MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
}