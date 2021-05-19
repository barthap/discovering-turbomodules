#include <jni.h>
#include "TurboUtilsModule.h"

#include <fbjni/fbjni.h>

using namespace facebook;

// C++ reference for Java class com.myturboutils.GreetingManager
struct JGreetingManager : jni::JavaClass<JGreetingManager> {
    static constexpr auto kJavaDescriptor = "Lcom/myturboutils/GreetingManager;";

    // C++ wrapper for constructior
    static jni::local_ref<JGreetingManager> create(std::string const& tag) {
        return newInstance(tag);
    }

    // calls GretingManager.makeGreeting()
    std::string prepareGreeting(std::string const& name) {
        static const auto method =
                getClass()->getMethod<jni::JString(std::string)>("makeGreeting");

        return method(self(), name)->toStdString();
    }
};

// this reflects com.myturboutils.NativeProxy class
struct NativeProxy : jni::JavaClass<NativeProxy> {
    static constexpr auto kJavaDescriptor = "Lcom/myturboutils/NativeProxy;";

    static void registerNatives() {
        // register native methods for Java
        javaClassStatic()->registerNatives({
            //used from Java
            makeNativeMethod("nativeSumSquares", NativeProxy::nativeSumSquares),
            makeNativeMethod("nativeMakeGreeting", NativeProxy::nativeMakeGreeting)
        });
    }

    /*** Methods below are called from Java ***/

    static jdouble nativeSumSquares(jni::alias_ref<jni::JClass> clazz, jdouble a, jdouble b) {
        return turboutils::sumSquares(a, b);
    }

    static std::string nativeMakeGreeting(jni::alias_ref<jni::JClass> clazz, jni::alias_ref<jni::JString> name) {
        /* a counterpart for Kotlin code:
         * val greetingManager = GreetingManager("fbjni")
         * return greetingManager.makeGreeting(name)
         */
        auto greetingManager = JGreetingManager::create("fbjni");
        return greetingManager->prepareGreeting(name->toStdString());
    }
};

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void*) {
    return jni::initialize(vm, [] {
        NativeProxy::registerNatives();
    });
}
