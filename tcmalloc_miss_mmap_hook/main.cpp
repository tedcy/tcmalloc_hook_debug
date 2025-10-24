#include <iostream>
#include <fstream>
#include <dlfcn.h>
#include <assert.h>

using namespace std;

#include <gperftools/heap-profiler.h>
int main() {
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

    string s = GetHeapProfile();
    HeapProfilerStop();

    fstream f;
    f.open("./allbin.hprof", ios_base::out);
    f << s;
    f.close();

    return 0;
}