#include "logs.h"

int main(int agrc, char **agrs) {
    LOGV("verbose");
    LOGD("debug");
    LOGI("info");
    LOGW("warning");
    LOGE("error");
    return 0;
}