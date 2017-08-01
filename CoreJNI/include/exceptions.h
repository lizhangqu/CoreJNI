#ifndef CORE_EXCEPTION_H
#define CORE_EXCEPTION_H

#include <jni.h>

void throwException(JNIEnv *env, const char *exceptionName, const char *exceptionMsg) {
    if (env == NULL) {
        return;
    }
    if (exceptionName == NULL) {
        return;
    }
    if (exceptionMsg == NULL) {
        return;
    }
    jclass exceptionClass = env->FindClass(exceptionName);
    if (exceptionClass) {
        env->ThrowNew(exceptionClass, exceptionMsg);
        env->DeleteLocalRef(exceptionClass);
    }
}

bool checkException(JNIEnv *env) {
    if (env == NULL) {
        return false;
    }
    if (env->ExceptionOccurred()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        return true;
    }
    return false;
}


bool checkAndThrowException(JNIEnv *env, const char *exceptionName, const char *exceptionMsg) {
    if (env == NULL) {
        return false;
    }
    if (env->ExceptionOccurred()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        throwException(env, exceptionName, exceptionMsg);
        return true;
    }
    return false;
}


#endif //CORE_EXCEPTION_H