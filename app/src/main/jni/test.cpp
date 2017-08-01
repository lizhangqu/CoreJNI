#include "jni.h"
#include "logs.h"
#include "strings.h"
#include "lists.h"
#include "maps.h"
#include "exceptions.h"
#include <iostream>
#include <typeinfo>
#include <stdexcept>
#include <exception>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <vector>

#include "ScopedLocalRef.h"
#include "ScopedUtfChars.h"
#include "ScopedStringChars.h"
#include "ScopedBytes.h"
#include "ScopedPrimitiveArray.h"
#include "UniquePtr.h"

#ifndef NELEM
# define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#endif

#ifndef CLASSNAME
#define CLASSNAME "io/github/lizhangqu/sample/Test"
#endif

/**
 * 段错误
 */
void doSigSEGVTest() {
    int *p = NULL;
    *p = 15;
}

double division(int a, int b) {
    if (b == 0) {
        throw "Division by zero condition!";
    }
    return (a / b);
}

void native_test(JNIEnv *env, jobject thiz) {
    LOGV("verbose");
    LOGD("debug");
    LOGI("info");
    LOGW("warning");
    LOGE("error");
    LOGE("%s hello", std::string("asas").c_str());

    jclass clazz = env->FindClass(CLASSNAME);
    jmethodID pID = env->GetStaticMethodID(clazz, "callByNative", "()V");
    env->CallStaticVoidMethod(clazz, pID);
    checkException(env);


    ScopedLocalRef<jclass> IllegalArgumentException(env, env->FindClass(
            "java/lang/IllegalArgumentException"));
    if (IllegalArgumentException.get() == NULL) {
        LOGE("can't get it");
    } else {
        LOGE("get it");
    }

    jstring test = env->NewStringUTF("test jstring");
    ScopedUtfChars scopedUtfChars(env, test);
    if (scopedUtfChars.c_str() == NULL) {
        LOGE("can't get it");
    } else {
        LOGE("get it %s", scopedUtfChars.c_str());
    }

    jstring jc = env->NewStringUTF("asasasasas");
    ScopedStringChars scopedStringChars(env, jc);
    if (scopedStringChars.get() == NULL) {
        LOGE("can't get it");
    } else {
        LOGE("get it %d", scopedStringChars.size());
    }

    UniquePtr<std::string> uniquePtr(new std::string("hello"));

    if (uniquePtr.get() == NULL) {
        LOGE("can't get it");
    } else {
        LOGE("get it %s", (*(uniquePtr.get())).c_str());
    }

    ScopedStringChars scopedStringChars1(env, NULL);
    if (env->ExceptionOccurred()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }

    throwException(env, "java/lang/IllegalArgumentException", "非法参数异常");


}

jstring native_string(JNIEnv *env, jobject thiz) {
    return javaString(env, "just a test");
}

void native_byte(JNIEnv *env, jobject thiz, jbyteArray array, jobject bytebuffer) {
    ScopedBytesRO scopedBytes(env, array);
    if (scopedBytes.get() == NULL) {
        LOGE("can't get it");
    } else {
        const jbyte *jbyte1 = scopedBytes.get();
        const unsigned char *tmp = reinterpret_cast<const unsigned char *>(jbyte1);
        LOGE("get it %s %d", tmp, scopedBytes.size());
    }

    ScopedBytesRO scopedByteBuffer(env, bytebuffer);
    if (scopedByteBuffer.get() == NULL) {
        LOGE("can't get it");
    } else {
        const jbyte *jbyte1 = scopedByteBuffer.get();
        const unsigned char *tmp = reinterpret_cast<const unsigned char *>(jbyte1);
        LOGE("get it %s %d", tmp, scopedByteBuffer.size());
    }

    ScopedByteArrayRO sourceBytes(env, array);
//    ScopedBooleanArrayRO RW
//    ScopedIntArrayRO RW
//    ScopedCharArrayRO RW
//    ScopedDoubleArrayRO RW
//    ScopedFloatArrayRO RW
//    ScopedLongArrayRO RW
//    ScopedShortArrayRO RW
    if (sourceBytes.get() == NULL) {
        LOGE("can't get it");
    } else {
        LOGE("get it %d", sourceBytes.size());
    }

}

jobject native_list(JNIEnv *env, jobject thiz) {
    std::vector<std::string> vec;
    vec.push_back("1234");
    vec.push_back("ada");
    vec.push_back("肯定不是");
    jobject pJobject = stlVectorToJavaList(env, vec);
    if (pJobject != NULL) {
        std::vector<std::string> vector = javaListToStlVector(env, pJobject);
        for (int i = 0; i < vector.size(); ++i) {
            LOGE("get from vector %s", vector[i].c_str());
        }
    }
    return pJobject;
}


jobject native_map(JNIEnv *env, jobject thiz, jobject javaMap) {
    std::map<std::string, std::string> myMap = javaMapToStlMap(env, javaMap);
    for (auto iter = myMap.begin(); iter != myMap.end(); ++iter) {
        auto myPair = *iter;
        std::string key = myPair.first;
        std::string value = myPair.second;

        LOGE("get from map key %s", key.c_str());
        LOGE("get from map value %s", value.c_str());
    }

    return stlMapToJavaMap(env, myMap);

}

static const JNINativeMethod sMethods[] = {
        {
                const_cast<char *>("native_test"),
                const_cast<char *>("()V"),
                reinterpret_cast<void *>(native_test)
        },
        {
                const_cast<char *>("native_string"),
                const_cast<char *>("()Ljava/lang/String;"),
                reinterpret_cast<void *>(native_string)
        },
        {
                const_cast<char *>("native_byte"),
                const_cast<char *>("([BLjava/nio/ByteBuffer;)V"),
                reinterpret_cast<void *>(native_byte)
        },
        {
                const_cast<char *>("native_list"),
                const_cast<char *>("()Ljava/util/List;"),
                reinterpret_cast<void *>(native_list)
        },
        {
                const_cast<char *>("native_map"),
                const_cast<char *>("(Ljava/util/Map;)Ljava/util/Map;"),
                reinterpret_cast<void *>(native_map)
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