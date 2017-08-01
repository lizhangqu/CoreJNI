#ifndef CORE_STRING_H
#define CORE_STRING_H

#include <jni.h>
#include <string>

jstring javaString(JNIEnv *env, const char *param) {
    if (env == NULL) {
        return NULL;
    }
    if (param == NULL) {
        return NULL;
    }
    int length = strlen(param);
    if (length == 0) {
        return NULL;
    }
    jclass java_lang_String = env->FindClass("java/lang/String");
    if (java_lang_String) {
        jmethodID java_lang_String_Constructor = env->GetMethodID(java_lang_String,
                                                                  "<init>",
                                                                  "([BLjava/lang/String;)V");
        if (java_lang_String_Constructor) {
            jbyteArray bytes = env->NewByteArray(length);
            env->SetByteArrayRegion(bytes, 0, length, (jbyte *) param);
            jstring uff8 = env->NewStringUTF("UTF-8");
            jstring result = (jstring) env->NewObject(java_lang_String,
                                                      java_lang_String_Constructor,
                                                      bytes,
                                                      uff8);
            env->DeleteLocalRef(java_lang_String);
            env->DeleteLocalRef(bytes);
            env->DeleteLocalRef(uff8);

            if (env->ExceptionOccurred()) {
                env->ExceptionDescribe();
                env->ExceptionClear();
                return NULL;
            }

            return result;
        }
    }
    return NULL;
}

#endif //CORE_STRING_H