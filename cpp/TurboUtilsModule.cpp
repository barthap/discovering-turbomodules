#include <jsi/jsilib.h>

#include "TurboUtilsModule.h"
#include "Logging.h"

using namespace facebook;

namespace turboutils
{
  double sumSquares(double a, double b) {
    return a*a + b*b;
  }

  /*** TurboModule implementation ***/
  void installTurboModule(jsi::Runtime& runtime, std::shared_ptr<react::CallInvoker> jsCallInvoker) {
      std::shared_ptr<UtilsTurboModule> nativeModule =
              std::make_shared<UtilsTurboModule>(jsCallInvoker);

      // register UtilsTurboModule instance as global._myUtilsTurboModule
      runtime.global().setProperty(
              runtime,
              jsi::PropNameID::forAscii(runtime, "_myUtilsTurboModule"),
              jsi::Object::createFromHostObject(runtime, nativeModule));
  }

    static jsi::Value
    _hostFunction_TurboUtilsSpecJSI_nativeSumSquares(jsi::Runtime &rt, react::TurboModule &turboModule,
                                                   const jsi::Value *args, size_t arg_count) {
        // We can do input validation here
        if (arg_count != 2 || !args[0].isNumber() || !args[1].isNumber()) {
            jsi::detail::throwJSError(rt, "Invalid input for sumSquares()");
            return {};
        }
        auto a = args[0].asNumber();
        auto b = args[1].asNumber();

        LOG("Calling UtilsTurboModule.sumSquares(%f, %f)", a, b);
        return dynamic_cast<TurboUtilsSpecJSI *>(&turboModule)->nativeSumSquares(rt, a, b);
    }

    static jsi::Value _hostFunction_TurboUtilsSpecJSI_nativeGreeting(jsi::Runtime &rt, react::TurboModule &turboModule,
                                                                const jsi::Value *args, size_t arg_count) {
        if (arg_count != 1 || !args[0].isString()) {
            jsi::detail::throwJSError(rt, "Invalid input for makeGreetingFor()");
            return {};
        }
        return dynamic_cast<TurboUtilsSpecJSI *>(&turboModule)->nativeGreeting(rt, args[0].getString(rt));
    }

    TurboUtilsSpecJSI::TurboUtilsSpecJSI(std::shared_ptr<react::CallInvoker> jsInvoker)
            : TurboModule("UtilsTurboModule", jsInvoker) {
        //here we assign our TurboModule object properties
        LOG("Initializing TurboModule MethodMap");
        methodMap_["makeGreetingFor"] = MethodMetadata{1, _hostFunction_TurboUtilsSpecJSI_nativeGreeting};
        methodMap_["sumSquares"] = MethodMetadata{2, _hostFunction_TurboUtilsSpecJSI_nativeSumSquares};
    }

    /**************************************************************************/

    jsi::String UtilsTurboModule::nativeGreeting(jsi::Runtime &rt, const jsi::String &name) {
        auto msg = std::string("Greeting not implemented yet for ") + name.utf8(rt);
        return jsi::String::createFromAscii(rt, msg);
    }

    jsi::Value UtilsTurboModule::nativeSumSquares(jsi::Runtime &rt, double a, double b) {
        return jsi::Value(sumSquares(a, b));
    }

    /**************************************************************************/


  // JSI non-turbomodule implementation
  void installJsi(jsi::Runtime& rt) {
      LOG("Installing JSI...");

      auto fnSumSquares
        = jsi::Function::createFromHostFunction(
                rt,
                jsi::PropNameID::forAscii(rt, "_sumSquares"), //name
                2,  //theoretically param count
                [](jsi::Runtime &rt,
                        const jsi::Value &thisVal,
                        const jsi::Value *args,
                        size_t arg_count) -> jsi::Value {
                    // We can do input validation here
                    if (arg_count != 2 || !args[0].isNumber() || !args[1].isNumber()) {
                        jsi::detail::throwJSError(rt, "Invalid input for sumSquares()");
                        return {};
                    }

                    auto a = args[0].asNumber();
                    auto b = args[1].asNumber();

                    LOG("Calling JSI sumSquares for %f, %f", a, b);
                    auto result = sumSquares(a, b);
                    return jsi::Value(result);
                });

      auto fnMakeGreeting
        = jsi::Function::createFromHostFunction(
                rt,
                jsi::PropNameID::forAscii(rt, "_makeGreeting"),
                1,
                [](jsi::Runtime &rt,
                        const jsi::Value &thisVal,
                        const jsi::Value *args,
                        size_t arg_count) -> jsi::Value {
                    if (arg_count != 1 || !args[0].isString()) {
                        jsi::detail::throwJSError(rt, "Invalid input for makeGreetingFor()");
                        return {};
                    }
                    // we're gonna do that later
                    auto name = args[0].asString(rt).utf8(rt);

                    LOG("Calling JSI makeGreeting for name: %s", name.c_str());
                    auto msg = std::string("Greeting not implemented yet for ") + name;
                    return jsi::String::createFromAscii(rt, msg);
                });

      // creates JS object: global._jsiTurboUtils = { sumSquares, makeGreetingFor };
      jsi::Object jsiModule = jsi::Object(rt);
      jsiModule.setProperty(rt, "sumSquares", std::move(fnSumSquares));
      jsiModule.setProperty(rt, "makeGreetingFor", std::move(fnMakeGreeting));

      rt.global().setProperty(rt, "_jsiTurboUtils", std::move(jsiModule));
  }
} // namespace turboutils
