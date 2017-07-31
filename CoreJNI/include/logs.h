#ifndef CORE_LOG_H
#define CORE_LOG_H

#ifdef NDEBUG

#ifndef LOGV
#define LOGV(...)
#endif

#ifndef LOGD
#define LOGD(...)
#endif

#ifndef LOGI
#define LOGI(...)
#endif

#ifndef LOGW
#define LOGW(...)
#endif

#ifndef LOGE
#define LOGE(...)
#endif

#else

#ifndef CORE_LOG_TAG
#define CORE_LOG_TAG "CoreJNI"
#endif

#ifdef ANDROID

#include <android/log.h>

#ifndef LOGV
#define LOGV(fmt, ...) __android_log_print(ANDROID_LOG_VERBOSE, CORE_LOG_TAG, fmt, ##__VA_ARGS__)
#endif

#ifndef LOGD
#define LOGD(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, CORE_LOG_TAG, fmt, ##__VA_ARGS__)
#endif

#ifndef LOGI
#define LOGI(fmt, ...) __android_log_print(ANDROID_LOG_INFO, CORE_LOG_TAG, fmt, ##__VA_ARGS__)
#endif

#ifndef LOGW
#define LOGW(fmt, ...) __android_log_print(ANDROID_LOG_WARN, CORE_LOG_TAG, fmt, ##__VA_ARGS__)
#endif

#ifndef LOGE
#define LOGE(fmt, ...) __android_log_print(ANDROID_LOG_ERROR, CORE_LOG_TAG, fmt, ##__VA_ARGS__)
#endif

#else

#include <stdio.h>

#ifndef LOGV
#define LOGV(...) printf("<%s>\t<%s>\t","verbose", CORE_LOG_TAG); printf(__VA_ARGS__); printf("\n");
#endif

#ifndef LOGD
#define LOGD(...) printf("< %s >\t<%s>\t","debug", CORE_LOG_TAG); printf(__VA_ARGS__); printf("\n");
#endif

#ifndef LOGI
#define LOGI(...) printf("< %s  >\t<%s>\t","info", CORE_LOG_TAG); printf(__VA_ARGS__); printf("\n");
#endif

#ifndef LOGW
#define LOGW(...) printf("<%s>\t<%s>\t","warning", CORE_LOG_TAG); printf(__VA_ARGS__); printf("\n");
#endif

#ifndef LOGE
#define LOGE(...) printf("< %s >\t<%s>\t","error", CORE_LOG_TAG); printf(__VA_ARGS__); printf("\n");
#endif

#endif

#endif

#endif //CORE_LOG_H