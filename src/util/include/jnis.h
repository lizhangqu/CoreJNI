#ifndef CORE_JNI_H
#define CORE_JNI_H

#include "logs.h"

#ifndef NELEM
# define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#endif

static int
CoreJNI_RegisterNativeMethods(JNIEnv *env, const char *className, const JNINativeMethod *methods,
                              const int numMethods) {
    jclass clazz = env->FindClass(className);
    if (!clazz) {
        LOGE("Native registration unable to find class '%s'\n", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, methods, numMethods) != 0) {
        LOGE("RegisterNatives failed for '%s'\n", className);
        env->DeleteLocalRef(clazz);
        return JNI_FALSE;
    }
    env->DeleteLocalRef(clazz);
    return JNI_TRUE;
}

static jint JNI_OnLoad(JavaVM *vm, void *reserved, const char *className,
                           const JNINativeMethod *nativeMethods, const int methodNumber) {
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        LOGE("Can't get JNIEnv for '%s'\n", className);
        return -1;
    }
    CoreJNI_RegisterNativeMethods(env, className, nativeMethods, methodNumber);
    return JNI_VERSION_1_6;
}


#endif //CORE_JNI_H