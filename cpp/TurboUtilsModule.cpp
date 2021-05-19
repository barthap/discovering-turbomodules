#include <jsi/jsilib.h>

#include "TurboUtilsModule.h"
#include "Logging.h"

#include <thread>
#include <chrono>
#include <future>

using namespace facebook;

namespace turboutils
{
  double sumSquares(double a, double b) {
    return a*a + b*b;
  }

  /*** TurboModule implementation ***/
  void installTurboModule(jsi::Runtime& runtime,
                          std::shared_ptr<react::CallInvoker> jsCallInvoker,
                          std::unique_ptr<PlatformAdapter> platformAdapter) {

      std::shared_ptr<UtilsTurboModule> nativeModule =
              std::make_shared<UtilsTurboModule>(jsCallInvoker, std::move(platformAdapter));

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

    // we need to store our thread reference outside all these lambdas
    // to avoid crashes when they die
    // this is a temporary solution; ideally a thread pool should be used
    static std::unique_ptr<std::thread> activeWorker;

    static jsi::Value _asyncFnExample(jsi::Runtime &rt, react::TurboModule &turboModule,
                                      const jsi::Value *args, size_t arg_count) {

      // this is used to schedule a JS function call on JS thread. (probably adds it to event loop queue)
      const auto callInvoker = dynamic_cast<TurboUtilsSpecJSI *>(&turboModule)->jsInvoker_;

      //a callback given as Promise constructor parameter
      //new Promise(callback), where the callback is (resolve, reject) => void;
      auto myFnCallback = jsi::Function::createFromHostFunction(
              rt,
              jsi::PropNameID::forAscii(rt, "myFnCallback"),
              2,
              [callInvoker](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
                  // resolve, reject params
                  auto resolverValue = std::make_shared<jsi::Value>((arguments[0].asObject(runtime)));
                  auto rejecterValue = std::make_shared<jsi::Value>((arguments[1].asObject(runtime)));

                  // create C++ callables for the JS param callbacks
                  auto resolver = [&runtime, resolverValue, callInvoker](const std::string& result) {
                      //invoke on JS thread a callback from param
                      callInvoker->invokeAsync([&runtime, resolverValue, result]() {
                          resolverValue->asObject(runtime).asFunction(runtime)
                            .call(runtime, jsi::String::createFromUtf8(runtime, result));
                      });
                  };
                  auto rejecter = [&runtime, rejecterValue, callInvoker](const std::string& message) {
                      callInvoker->invokeAsync([&runtime, rejecterValue, message]() {
                          rejecterValue->asObject(runtime)
                            .asFunction(runtime)
                            .call(runtime, jsi::JSError(runtime, message).value());
                      });
                  };

                  // a separate thread action
                  auto job = [resolver, rejecter](){
                      LOG("inside thread");
                      std::this_thread::sleep_for(std::chrono::seconds(2));
                      LOG("after 2s");
                      resolver("HelloResult");
                  };
                  std::packaged_task<void(void)> bgJob(job);

                  // wait for previous job if hasn't finished yet
                  // when it has already finished, join() returns right away
                  if (activeWorker) {
                      LOG("previous thread may have not finished yet, waiting...");
                      activeWorker->join();
                      // this solution has one drawback: the UI hangs until that thread finishes
                      // a thread pool should solve this problem
                  }

                  // it starts a thread
                  LOG("starting thread...");
                  activeWorker = std::make_unique<std::thread>(std::move(bgJob));

                  //Promise callback is void type (Return undefined)
                  LOG("thread started, returning...");
                  return jsi::Value::undefined();
              });

      // return Promise
        auto newPromise = rt.global().getProperty(rt, "Promise");
        auto promise = newPromise
                .asObject(rt)
                .asFunction(rt)
                .callAsConstructor(rt, myFnCallback);

        return promise;
    }

    TurboUtilsSpecJSI::TurboUtilsSpecJSI(std::shared_ptr<react::CallInvoker> jsInvoker)
            : TurboModule("UtilsTurboModule", jsInvoker) {
        //here we assign our TurboModule object properties
        LOG("Initializing TurboModule MethodMap");
        methodMap_["makeGreetingFor"] = MethodMetadata{1, _hostFunction_TurboUtilsSpecJSI_nativeGreeting};
        methodMap_["sumSquares"] = MethodMetadata{2, _hostFunction_TurboUtilsSpecJSI_nativeSumSquares};
        methodMap_["funAsync"] = MethodMetadata{0, _asyncFnExample};
    }

    /**************************************************************************/

    jsi::String UtilsTurboModule::nativeGreeting(jsi::Runtime &rt, const jsi::String &name) {
        auto result = this->_platformAdapter->delegateGreeting(name.utf8(rt));
        return jsi::String::createFromAscii(rt, result);
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
