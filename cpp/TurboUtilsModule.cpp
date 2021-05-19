#include <jsi/jsilib.h>

#include "TurboUtilsModule.h"
#include "Logging.h"

using namespace facebook;

namespace turboutils
{
  double sumSquares(double a, double b) {
    return a*a + b*b;
  }


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
