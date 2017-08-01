#include "logs.h"


typedef struct {
    const char *name;
    const char *signature;
    void *fnPtr;
} JNINativeMethod;

void native_test() {

}


#ifndef NELEM
# define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#endif

template<class T, size_t N>
constexpr size_t test_size(T (&)[N]) { return N; }

static void CoreJNI_OnLoad( int nativeMethods[5]) {
    int n = test_size(nativeMethods);
    LOGE("%d", n);
}


int main(int agrc, char **agrs) {
    int sMethods[] = {
            2,
            3,
            4,
            5
    };
    int n = ((int) (sizeof(sMethods) / sizeof((sMethods)[0])));
    LOGE("%d", n);
    CoreJNI_OnLoad(sMethods);
    LOGE("%d", NELEM(sMethods));
    return 0;
}