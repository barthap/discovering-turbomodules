#pragma once

#define TAG "MyTurboUtils-CPP"

#ifdef __ANDROID__
#include <android/log.h>

#define LOG(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

#else // iOS

#define LOG(...) printf("[%s]: ", TAG); printf(__VA_ARGS__); printf("\n")

#endif //__ANDROID__

