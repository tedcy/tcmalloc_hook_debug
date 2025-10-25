#include <unistd.h>
#include <sys/mman.h>
int main() {
    sbrk(10);
    mmap(NULL, 4 * 1024 * 1024, PROT_READ | PROT_WRITE,
        MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    return 0;
}