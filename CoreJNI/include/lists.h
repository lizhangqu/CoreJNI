#ifndef CORE_LIST_H
#define CORE_LIST_H

#include <jni.h>
#include <string>
#include <vector>

jobject stlVectorToJavaList(JNIEnv *env, std::vector<std::string> param) {
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

std::vector<std::string> javaListToStlVector(JNIEnv *env, jobject list) {
    std::vector<std::string> result;
    if (env == NULL) {
        return result;
    }
    if (list == NULL) {
        return result;
    }
    jclass java_util_List = env->FindClass("java/util/List");
    if (java_util_List == NULL) {
        return result;
    }
    jmethodID java_util_List_size = env->GetMethodID(java_util_List, "size", "()I");
    jmethodID java_util_List_get = env->GetMethodID(java_util_List, "get",
                                                    "(I)Ljava/lang/Object;");

    jint size = env->CallIntMethod(list, java_util_List_size);
    for (jint i = 0; i < size; i++) {
        jstring element = (jstring) env->CallObjectMethod(list, java_util_List_get, i);
        char *temp = (char *) env->GetStringUTFChars(element, NULL);
        result.push_back(std::string(temp));
        env->ReleaseStringUTFChars(element, temp);
        env->DeleteLocalRef(element);
    }
    env->DeleteLocalRef(java_util_List);
    return result;
}

#endif //CORE_LIST_H