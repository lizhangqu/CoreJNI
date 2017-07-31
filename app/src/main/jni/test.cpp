#include "jni.h"
#include "logs.h"

#ifndef NELEM
# define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#endif

#ifndef CLASSNAME
#define CLASSNAME "io/github/lizhangqu/sample/Test"
#endif

void native_test(JNIEnv *env, jobject thiz) {
    LOGV("verbose");
    LOGD("debug");
    LOGI("info");
    LOGW("warning");
    LOGE("error");
}

static const JNINativeMethod sMethods[] = {
        {
                const_cast<char *>("native_test"),
                const_cast<char *>("()V"),
                reinterpret_cast<void *>(native_test)
        }
};

int registerNativeMethods(JNIEnv *env, const char *className, const JNINativeMethod *methods,
                          const int numMethods) {
    jclass clazz = env->FindClass(className);
    if (!clazz) {
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, methods, numMethods) != 0) {
        env->DeleteLocalRef(clazz);
        return JNI_FALSE;
    }
    env->DeleteLocalRef(clazz);
    return JNI_TRUE;
}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    registerNativeMethods(env, CLASSNAME, sMethods, NELEM(sMethods));
    return JNI_VERSION_1_6;
}