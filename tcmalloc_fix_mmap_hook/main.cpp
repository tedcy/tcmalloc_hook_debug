// main.cpp
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <dlfcn.h>
#include "PFishHook/PFishHook.h"
#include <assert.h>

using namespace std;

#define DEFINE_HOOK_FUNC(name) \
    using name##_t = decltype(&name); \
    static name##_t name##_f = nullptr;

#include <gperftools/tcmalloc.h>
#include <sys/mman.h>
extern "C" void* sbrk(intptr_t increment);
DEFINE_HOOK_FUNC(sbrk)

void* getGlibc() {
    const char* library_path = "/lib/x86_64-linux-gnu/libc.so.6";
    void* handle = dlopen(library_path, RTLD_LOCAL | RTLD_DEEPBIND | RTLD_NOW);
    if (!handle) {
        cerr << "Failed to open library: " << dlerror() << endl;
        quick_exit(0);
    }
    return handle;
}

//RTLD_DEFAULT是tc_malloc的，RTLD_NEXT是glibc的
#define HOOK_FUNC(libc_func) \
    do { \
        void *f = dlsym(getGlibc(), #libc_func); \
        assert(f); \
        simple_hook(f, (void*)tc_##libc_func); \
    } while (0)

#define HOOK_FUNC_RENAME(libc_func, tcmalloc_func) \
    do { \
        void *f = dlsym(getGlibc(), #libc_func); \
        assert(f); \
        simple_hook(f, (void*)tcmalloc_func); \
    } while (0)

#define HOOK_FUNC_RENAME_KEEP_OLD(libc_func, tcmalloc_func) \
    do { \
        void *f = dlsym(getGlibc(), #libc_func); \
        assert(f); \
        HookIt(f, (void**)&libc_func##_f, (void*)tcmalloc_func); \
        if (!libc_func##_f) { \
            cout << "hook " #libc_func " failed" << std::endl; \
            std::quick_exit(0); \
        } \
    } while (0)

#define HOOK_FUNC_NEED_FIND(libc_func) \
    do { \
        void *f1 = dlsym(getGlibc(), #libc_func);\
        assert(f1); \
        void *f2 = dlsym(RTLD_DEFAULT, #libc_func);\
        assert(f2); \
        assert(f1 != f2);\
        simple_hook(f1, f2); \
    } while (0)

#include <gperftools/malloc_hook.h>

void MallocHook::InvokePreSbrkHook(ptrdiff_t increment) {
    InvokePreSbrkHookSlow(increment);
}
void MallocHook::InvokeSbrkHook(const void* result, ptrdiff_t increment) {
    InvokeSbrkHookSlow(result, increment);
}
extern "C" void *my_sbrk(intptr_t increment) __THROW {
    MallocHook::InvokePreSbrkHook(increment);
    void *result = sbrk_f(increment);
    MallocHook::InvokeSbrkHook(result, increment);
    return result;
}

extern void simple_hook(void *sym, void* targetFunc);
void hookAll() {
    HOOK_FUNC(malloc);
    HOOK_FUNC(free);
    HOOK_FUNC(realloc);
    HOOK_FUNC(calloc);
    HOOK_FUNC(cfree);
    HOOK_FUNC(memalign);
    HOOK_FUNC(posix_memalign);
    HOOK_FUNC(valloc);
    HOOK_FUNC(pvalloc);
    HOOK_FUNC(malloc_stats);
    HOOK_FUNC(mallopt);
    HOOK_FUNC_RENAME(malloc_usable_size, tc_malloc_size);
    //glibc_mmap->tcmalloc_mmap->syscall_mmap
    HOOK_FUNC_NEED_FIND(mmap);
    HOOK_FUNC_NEED_FIND(mmap64);
    HOOK_FUNC_NEED_FIND(munmap);
    HOOK_FUNC_NEED_FIND(mremap);
    //new_glibc_sbrk->tcmalloc_sbrk->old_glibc_sbrk
    HOOK_FUNC_RENAME_KEEP_OLD(sbrk, my_sbrk);
}

namespace FLAG__namespace_do_not_use_directly_use_DECLARE_bool_instead {
    extern bool FLAGS_mmap_profile;
}

#include <gperftools/heap-profiler.h>
#include <gperftools/malloc_extension.h>
int main() {
    FLAG__namespace_do_not_use_directly_use_DECLARE_bool_instead::FLAGS_mmap_profile = true;
    hookAll();

    void *handle =
        dlopen("./libdynamic.so", RTLD_NOW | RTLD_DEEPBIND | RTLD_LOCAL);
    if (!handle) {
        return 1;
    }
    using testFuncT = void*(*)();
    testFuncT testFunc = (testFuncT)dlsym(handle, "test");
    if (!testFunc) {
        return 1;
    }

    HeapProfilerStart("");

    for (int i = 0;i < 10;i++) {
        testFunc();
    }

    // HeapProfilerDump("");

    string s = GetHeapProfile();
    // MallocExtension::instance()->GetHeapSample(&s);
    HeapProfilerStop();

    fstream f;
    f.open("./allbin.hprof", ios_base::out);
    f << s;
    f.close();

    return 0;
}