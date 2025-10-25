#include <sys/mman.h>
#include <dlfcn.h>
#include <cstring>
#include <stdlib.h>
#include <stdint.h>

// 把原函数的前14字节改成绝对间接跳转到目标函数
void simple_hook(void *sym, void* targetFunc) {
    unsigned char patch[14] = {0xFF, 0x25, 0x00, 0x00, 0x00, 0x00};
    memcpy(&patch[6], &targetFunc, 8);

    // 改写页面权限（mprotect需要4K对齐），写入，再恢复权限
    void* pstart = reinterpret_cast<void*>(reinterpret_cast<uint64_t>(sym) &
                                           0xFFFFFFFFFFFFF000);

    if (mprotect(pstart, 4096, PROT_READ | PROT_WRITE | PROT_EXEC) != 0)
        abort();

    memcpy(sym, patch, sizeof(patch));

    if (mprotect(pstart, 4096, PROT_READ | PROT_EXEC) != 0)
        abort();
}

void simple_hook(const char* originFunc, void* targetFunc) {
    void* sym = dlsym(RTLD_DEFAULT, originFunc);
    if (!sym) abort();
    simple_hook(sym, targetFunc);
}