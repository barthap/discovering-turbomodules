#include <jni.h>
#include "TurboUtilsModule.h"

extern "C"
JNIEXPORT jdouble JNICALL
Java_com_myturboutils_MyTurboUtilsModule_nativeSumSquares(JNIEnv *env, jclass type, jdouble a, jdouble b) {
    return turboutils::sumSquares(a, b);
}
