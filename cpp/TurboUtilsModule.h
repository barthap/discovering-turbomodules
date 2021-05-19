#ifndef TURBOUTILSMODULE_H
#define TURBOUTILSMODULE_H

#include <jsi/jsi.h>

#ifdef __ANDROID__
  #include <TurboModule.h>
#else
  #include <ReactCommon/TurboModule.h>
#endif

#include "PlatformAdapter.h"

namespace turboutils {
  using namespace facebook;

  double sumSquares(double a, double b);

  void installJsi(jsi::Runtime& rt);

  /****** TURBO MODULE STUFF BELOW *******/
  void installTurboModule(jsi::Runtime& runtime,
                          std::shared_ptr<react::CallInvoker> jsCallInvoker,
                          std::unique_ptr<PlatformAdapter> platformAdapter);

  // This abstract class defines JSI interfaces for the turbo module
  class JSI_EXPORT TurboUtilsSpecJSI : public facebook::react::TurboModule {
  protected:
      TurboUtilsSpecJSI(std::shared_ptr<facebook::react::CallInvoker> jsInvoker);

  public:
      // define our interface methods
      virtual jsi::String nativeGreeting(jsi::Runtime &rt, const jsi::String &name) = 0;
      virtual jsi::Value nativeSumSquares(jsi::Runtime &rt, double a, double b) = 0;

  };

  // This is the actual implementation of the module methods
    class UtilsTurboModule : public TurboUtilsSpecJSI {
      std::unique_ptr<PlatformAdapter> _platformAdapter;
    public:
        UtilsTurboModule(std::shared_ptr<react::CallInvoker> jsInvoker,
                         std::unique_ptr<PlatformAdapter> platformAdapter)
                : TurboUtilsSpecJSI(jsInvoker), _platformAdapter(std::move(platformAdapter)) {}

        jsi::String nativeGreeting(jsi::Runtime &rt, const jsi::String &name) override;
        jsi::Value nativeSumSquares(jsi::Runtime &rt, double a, double b) override;
    };
}

#endif /* TURBOUTILSMODULE_H */
