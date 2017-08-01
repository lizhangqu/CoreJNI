#ifndef CORE_MAP_H
#define CORE_MAP_H

#include <jni.h>
#include <string>
#include <map>

std::map<std::string, std::string> javaMapToSTLMap(JNIEnv *env, jobject javaMap) {
    std::map<std::string, std::string> stlMap;
    if (env == NULL) {
        return stlMap;
    }
    jclass java_util_Map = env->FindClass("java/util/Map");
    jclass java_util_Set = env->FindClass("java/util/Set");
    if (java_util_Map == NULL) {
        return stlMap;
    }
    if (java_util_Set == NULL) {
        return stlMap;
    }
    jmethodID java_util_Map_keySet = env->GetMethodID(java_util_Map, "keySet", "()Ljava/util/Set;");
    jmethodID java_util_Map_get = env->GetMethodID(java_util_Map, "get",
                                                   "(Ljava/lang/Object;)Ljava/lang/Object;");
    jmethodID java_util_Set_toArray = env->GetMethodID(java_util_Set, "toArray",
                                                       "()[Ljava/lang/Object;");

    jobject jValue_keySet = env->CallObjectMethod(javaMap, java_util_Map_keySet);
    jobjectArray jValue_toArray = (jobjectArray) env->CallObjectMethod(jValue_keySet,
                                                                       java_util_Set_toArray);
    jsize arraySize = env->GetArrayLength(jValue_toArray);
    for (int i = 0; i < arraySize; i++) {
        jstring jkey = (jstring) env->GetObjectArrayElement(jValue_toArray, i);
        jstring jvalue = (jstring) env->CallObjectMethod(javaMap, java_util_Map_get, jkey);
        const char *key = env->GetStringUTFChars(jkey, NULL);
        const char *value = env->GetStringUTFChars(jvalue, NULL);
        stlMap.insert(std::make_pair(std::string(key), std::string(value)));
        env->ReleaseStringUTFChars(jkey, key);
        env->ReleaseStringUTFChars(jvalue, value);
        env->DeleteLocalRef(jkey);
        env->DeleteLocalRef(jvalue);
    }
    env->DeleteLocalRef(jValue_toArray);
    env->DeleteLocalRef(jValue_keySet);
    env->DeleteLocalRef(java_util_Map);
    env->DeleteLocalRef(java_util_Set);
    return stlMap;
}

#endif //CORE_MAP_H