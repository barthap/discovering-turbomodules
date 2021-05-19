#include <jni.h>
#include <CallInvokerHolder.h>
#include <fbjni/fbjni.h>
#include <jsi/jsi.h>

#include "PlatformAdapter.h"
#include "TurboUtilsModule.h"
#include "Logging.h"


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

// Delegation of platform-specific code from JSI TurboModule
class AndroidAdapter : public turboutils::PlatformAdapter {
    jni::global_ref<JGreetingManager> _greetingManager;
public:
    AndroidAdapter() {
        auto recognizer = JGreetingManager::create("JNI");
        _greetingManager = jni::make_global(recognizer);
    }

    std::string delegateGreeting(const std::string &name) override {
        return _greetingManager->prepareGreeting(name);
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
            makeNativeMethod("nativeMakeGreeting", NativeProxy::nativeMakeGreeting),

            //initialization for JSI
            makeNativeMethod("installNativeJsi", NativeProxy::installNativeJsi)
        });
    }

private:
    static void installNativeJsi(jni::alias_ref<jni::JObject> thiz,
                                 jlong jsiRuntimePtr,
                                 jni::alias_ref<react::CallInvokerHolder::javaobject> jsCallInvokerHolder) {

        auto jsiRuntime = reinterpret_cast<jsi::Runtime*>(jsiRuntimePtr);
        auto jsCallInvoker = jsCallInvokerHolder->cthis()->getCallInvoker();
        auto platformAdapter = std::make_unique<AndroidAdapter>();

        // initialize jsi module
        turboutils::installJsi(*jsiRuntime);

        // initialize turbo module
        turboutils::installTurboModule(*jsiRuntime, jsCallInvoker, std::move(platformAdapter));
    }

private:
    /*** Methods below are called from Java ***/

    static jdouble nativeSumSquares(jni::alias_ref<jni::JClass> clazz, jdouble a, jdouble b) {
        return turboutils::sumSquares(a, b);
    }

    static std::string nativeMakeGreeting(jni::alias_ref<jni::JClass> clazz, jni::alias_ref<jni::JString> name) {
        /* a counterpart for Kotlin code:
         * val greetingManager = GreetingManager("fbjni")
         * return greetingManager.makeGreeting(name)
         */
        LOG("Creating and calling Java GreetingManager instance");
        auto greetingManager = JGreetingManager::create("fbjni");
        return greetingManager->prepareGreeting(name->toStdString());
    }
};

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void*) {
    return jni::initialize(vm, [] {
        NativeProxy::registerNatives();
    });
}
