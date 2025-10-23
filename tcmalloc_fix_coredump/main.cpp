#include <iostream>
#include <gperftools/tcmalloc.h>
#include <dlfcn.h>

extern "C" void freeMemory(void* ptr);

using namespace std;

void my_free(void* ptr) {
    tc_free(ptr);
}

extern void simple_hook(void *sym, void* targetFunc);
void hookGlibc() {
    const char* library_path = "/lib/x86_64-linux-gnu/libc.so.6";
    void* handle = dlopen(library_path, RTLD_LOCAL | RTLD_DEEPBIND | RTLD_NOW);
    if (!handle) {
        cerr << "Failed to open library: " << dlerror() << endl;
        quick_exit(0);
    }
    void* symbol = dlsym(handle, "free");
    // void* symbol = dlsym(RTLD_DEFAULT,"free"); => 指向tc_free
    // void* symbol = dlsym(RTLD_NEXT, "free"); => 指向glibc free
    if (symbol) {
        cout << "Symbol found in " << library_path << " at address: " << symbol << endl;
    } else {
        cerr << "Failed to find symbol in " << library_path << ": " << dlerror() << endl;
        quick_exit(0);
    }
    simple_hook(symbol, (void*)my_free);
}

int main() {
    hookGlibc();

    // 主程序tcmalloc申请，动态库dlopen RTLD_DEEPBIND打开的释放
    // 这里为了不会被dlopen缓存，重新复制了一个动态库
    void *ptr = malloc(128);
    if (!ptr) {
        return 1;
    }
    // 这里去掉RTLD_DEEPBIND，就不需要hook了
    void* handle = dlopen("./libdynamic.so", RTLD_NOW | RTLD_DEEPBIND | RTLD_LOCAL);
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