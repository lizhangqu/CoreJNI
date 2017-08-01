#ifndef CORE_MAP_H
#define CORE_MAP_H

#include <jni.h>
#include <string>
#include <map>

jobject stlMapToJavaMap(JNIEnv *env, std::map<std::string, std::string> stlMap) {
    if (env == NULL) {
        return NULL;
    }
    jclass java_util_HashMap = env->FindClass("java/util/HashMap");
    if (java_util_HashMap == NULL) {
        return NULL;
    }
    jmethodID java_util_HashMap_init = env->GetMethodID(java_util_HashMap, "<init>", "()V");
    jmethodID java_util_HashMap_put = env->GetMethodID(java_util_HashMap, "put",
                                                       "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    jobject jMap = env->NewObject(java_util_HashMap, java_util_HashMap_init);

    for (auto iter = stlMap.begin(); iter != stlMap.end(); ++iter) {
        jstring jKey = env->NewStringUTF((*iter).first.c_str());
        jstring jValue = env->NewStringUTF((*iter).second.c_str());
        env->CallObjectMethod(jMap, java_util_HashMap_put, jKey, jValue);
        env->DeleteLocalRef(jKey);
        env->DeleteLocalRef(jValue);
    }
    env->DeleteLocalRef(java_util_HashMap);
    return jMap;
}


std::map<std::string, std::string> javaMapToStlMap(JNIEnv *env, jobject javaMap) {
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