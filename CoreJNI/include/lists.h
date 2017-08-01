#ifndef CORE_LIST_H
#define CORE_LIST_H

#include <jni.h>
#include <string>
#include <vector>

jobject javaListForString(JNIEnv *env, std::vector<std::string> param) {
    if (env == NULL) {
        return NULL;
    }
    jclass java_util_ArrayList = env->FindClass("java/util/ArrayList");
    if (java_util_ArrayList == NULL) {
        return NULL;
    }
    jmethodID java_util_ArrayList_init = env->GetMethodID(java_util_ArrayList, "<init>", "()V");
    jmethodID java_util_ArrayList_add = env->GetMethodID(java_util_ArrayList, "add",
                                                         "(Ljava/lang/Object;)Z");
    jobject list = env->NewObject(java_util_ArrayList, java_util_ArrayList_init);

    for (std::string item: param) {
        jstring element = env->NewStringUTF(item.c_str());
        env->CallBooleanMethod(list, java_util_ArrayList_add, element);
        env->DeleteLocalRef(element);
    }
    env->DeleteLocalRef(java_util_ArrayList);
    return list;
}

#endif //CORE_LIST_H