/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * JNI helper functions.
 *
 * This file may be included by C or C++ code, which is trouble because jni.h
 * uses different typedefs for JNIEnv in each language.
 *
 * TODO: remove C support.
 */
#ifndef NATIVEHELPER_COMMON_H_
#define NATIVEHELPER_COMMON_H_

#include "jni.h"
#include "android/log.h"

#define INNER_LOG(fmt, ...) __android_log_print(ANDROID_LOG_VERBOSE, "Common", fmt, ##__VA_ARGS__)

#ifndef NELEM
# define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#endif

#if !defined(DISALLOW_COPY_AND_ASSIGN)
// DISALLOW_COPY_AND_ASSIGN disallows the copy and operator= functions. It goes in the private:
// declarations in a class.
#if __cplusplus >= 201103L
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;  \
  void operator=(const TypeName&) = delete
#else
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);  \
  void operator=(const TypeName&)
#endif  // __has_feature(cxx_deleted_functions)
#endif  // !defined(DISALLOW_COPY_AND_ASSIGN)


/**
 * Equivalent to ScopedLocalRef
 */
template<typename T>
class scoped_local_ref {
public:
    explicit scoped_local_ref(JNIEnv *env, T localRef = NULL)
            : mEnv(env), mLocalRef(localRef) {
    }

    ~scoped_local_ref() {
        reset();
    }

    void reset(T localRef = NULL) {
        if (mLocalRef != NULL) {
            mEnv->DeleteLocalRef(mLocalRef);
            mLocalRef = localRef;
        }
    }

    T get() const {
        return mLocalRef;
    }

private:
    JNIEnv *const mEnv;
    T mLocalRef;

    DISALLOW_COPY_AND_ASSIGN(scoped_local_ref);
};

/*
 * Returns a human-readable summary of an exception object.  The buffer will
 * be populated with the "binary" class name and, if present, the
 * exception message.
 */
static bool getExceptionSummary(JNIEnv *env, jthrowable exception, std::string &result) {

    /* get the name of the exception's class */
    scoped_local_ref<jclass> exceptionClass(env, env->GetObjectClass(exception)); // can't fail
    scoped_local_ref<jclass> classClass(env, env->GetObjectClass(
            exceptionClass.get())); // java.lang.Class, can't fail
    jmethodID classGetNameMethod =
            env->GetMethodID(classClass.get(), "getName", "()Ljava/lang/String;");
    scoped_local_ref<jstring> classNameStr(env,
                                           (jstring) env->CallObjectMethod(
                                                   exceptionClass.get(),
                                                   classGetNameMethod));
    if (classNameStr.get() == NULL) {
        env->ExceptionClear();
        result = "<error getting class name>";
        return false;
    }
    const char *classNameChars = env->GetStringUTFChars(classNameStr.get(), NULL);
    if (classNameChars == NULL) {
        env->ExceptionClear();
        result = "<error getting class name UTF-8>";
        return false;
    }
    result += classNameChars;
    env->ReleaseStringUTFChars(classNameStr.get(), classNameChars);

    /* if the exception has a detail message, get that */
    jmethodID getMessage =
            env->GetMethodID(exceptionClass.get(), "getMessage", "()Ljava/lang/String;");
    scoped_local_ref<jstring> messageStr(env,
                                         (jstring) env->CallObjectMethod(exception,
                                                                         getMessage));
    if (messageStr.get() == NULL) {
        return true;
    }

    result += ": ";

    const char *messageChars = env->GetStringUTFChars(messageStr.get(), NULL);
    if (messageChars != NULL) {
        result += messageChars;
        env->ReleaseStringUTFChars(messageStr.get(), messageChars);
    } else {
        result += "<error getting message>";
        env->ExceptionClear(); // clear OOM
    }

    return true;
}


int jniThrowException(JNIEnv *env, const char *className, const char *msg) {

    if (env->ExceptionCheck()) {
        scoped_local_ref<jthrowable> exception(env, env->ExceptionOccurred());
        env->ExceptionClear();

        if (exception.get() != NULL) {
            std::string text;
            getExceptionSummary(env, exception.get(), text);
            INNER_LOG("Discarding pending exception (%s) to throw %s", text.c_str(),
                      className);
        }
    }

    scoped_local_ref<jclass> exceptionClass(env, env->FindClass(className));
    if (exceptionClass.get() == NULL) {
        INNER_LOG("Unable to find exception class %s", className);
        /* ClassNotFoundException now pending */
        return -1;
    }

    if (env->ThrowNew(exceptionClass.get(), msg) != JNI_OK) {
        INNER_LOG("Failed throwing '%s' '%s'", className, msg);
        /* an exception, most likely OOM, will now be pending */
        return -1;
    }

    return 0;
}

int jniThrowNullPointerException(JNIEnv *env, const char *msg) {
    return jniThrowException(env, "java/lang/NullPointerException", msg);
}


#endif  /* NATIVEHELPER_COMMON_H_ */
