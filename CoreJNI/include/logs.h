#ifndef CORE_LOG_H
#define CORE_LOG_H

#ifdef NDEBUG
#define LOGV(...)
#define LOGD(...)
#define LOGI(...)
#define LOGW(...)
#define LOGE(...)

#else

#ifndef CORE_LOG_TAG
#define CORE_LOG_TAG "CoreJNI"
#endif

#ifdef ANDROID

#include <android/log.h>

#define LOGV(fmt, ...) __android_log_print(ANDROID_LOG_VERBOSE, CORE_LOG_TAG, fmt, ##__VA_ARGS__)
#define LOGD(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, CORE_LOG_TAG, fmt, ##__VA_ARGS__)
#define LOGI(fmt, ...) __android_log_print(ANDROID_LOG_INFO, CORE_LOG_TAG, fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) __android_log_print(ANDROID_LOG_WARN, CORE_LOG_TAG, fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...) __android_log_print(ANDROID_LOG_ERROR, CORE_LOG_TAG, fmt, ##__VA_ARGS__)
#else

#include <stdio.h>

#define LOGV(...) printf("<%s>\t<%s>\t","verbose", CORE_LOG_TAG); printf(__VA_ARGS__); printf("\n");
#define LOGD(...) printf("< %s >\t<%s>\t","debug", CORE_LOG_TAG); printf(__VA_ARGS__); printf("\n");
#define LOGI(...) printf("< %s  >\t<%s>\t","info", CORE_LOG_TAG); printf(__VA_ARGS__); printf("\n");
#define LOGW(...) printf("<%s>\t<%s>\t","warning", CORE_LOG_TAG); printf(__VA_ARGS__); printf("\n");
#define LOGE(...) printf("< %s >\t<%s>\t","error", CORE_LOG_TAG); printf(__VA_ARGS__); printf("\n");
#endif

#endif

#endif //CORE_LOG_H