#include <gperftools/tcmalloc.h>
#include <dlfcn.h>

extern "C" void freeMemory(void* ptr);

using namespace std;

void my_free(void* ptr) {
    tc_free(ptr);
}

int main() {
    // 1) 主程序tcmalloc申请和释放
    void* ptr = malloc(128);
    if (!ptr) {
        return 1;
    }
    free(ptr);

    // 2) 主程序tcmalloc申请，动态库非dlopen RTLD_DEEPBIND打开的释放
    ptr = malloc(128);
    if (!ptr) {
        return 1;
    }
    // 将内存交给动态库释放
    freeMemory(ptr);
    
    // 3) 主程序tcmalloc申请，动态库dlopen RTLD_DEEPBIND打开的释放
    // 这里为了不会被dlopen缓存，重新复制了一个动态库
    ptr = malloc(128);
    if (!ptr) {
        return 1;
    }
    // 这里去掉RTLD_DEEPBIND，就不需要hook了
    void* handle = dlopen("./libdynamic1.so", RTLD_NOW | RTLD_DEEPBIND | RTLD_LOCAL);
    if (!handle) {
        return 1;
    }
    using FreeMemoryFuncT = decltype(&freeMemory);
    FreeMemoryFuncT freeMemory1 = (FreeMemoryFuncT)dlsym(handle, "freeMemory");
    if (!freeMemory1) {
        return 1;
    }
    freeMemory1(ptr);

    return 0;
}