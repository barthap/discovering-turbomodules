#import "MyTurboUtils.h"
#import "IosTurboUtilsAdapter.h"
#import "my_turbo_utils-Swift.h"

#include "TurboUtilsModule.h"
#include "Logging.h"

#import <React/RCTBridge+Private.h>
#import <React/RCTUtils.h>
#import <ReactCommon/RCTTurboModuleManager.h>
#import <jsi/jsi.h>
#import <memory>

using namespace facebook;

@implementation MyTurboUtils
@synthesize bridge=_bridge;
@synthesize methodQueue = _methodQueue;

RCT_EXPORT_MODULE()

+ (BOOL)requiresMainQueueSetup {
  return YES;
}

- (void)setBridge:(RCTBridge *)bridge {
  _bridge = bridge;
  
  LOG("JSI Module initialization in setBridge()");

  RCTCxxBridge *cxxBridge = (RCTCxxBridge *)self.bridge;
  if (!cxxBridge.runtime) {
    return;
  }
  
  jsi::Runtime* jsiRuntime = (jsi::Runtime *)cxxBridge.runtime;
  auto callInvoker = bridge.jsCallInvoker;
  auto platformAdapter = std::make_unique<IosAdapter>();

  // init jsi-based module
  turboutils::installJsi(*jsiRuntime);
  
  // init turbo module
  turboutils::installTurboModule(*jsiRuntime, callInvoker, std::move(platformAdapter));
}


#pragma mark - Legacy Bridge Methods

RCT_EXPORT_METHOD(sumSquares:(nonnull NSNumber*)a withB:(nonnull NSNumber*)b
                  withResolver:(RCTPromiseResolveBlock)resolve
                  withReject:(RCTPromiseRejectBlock)reject)
{
    NSNumber *result = @(turboutils::sumSquares([a doubleValue], [b doubleValue]));

    resolve(result);
}

RCT_EXPORT_METHOD(makeGreetingFor:(nonnull NSString*)name
                  withResolver:(RCTPromiseResolveBlock)resolve
                  withReject:(RCTPromiseRejectBlock)reject)
{
  // this is not optimal, but we don't care about legacy bridge
  GreetingManager* greetingManager = [[GreetingManager alloc] initWithTag:@"iOS Bridge"];
  NSString* result = [greetingManager makeGreetingForName:name];
  
  resolve(result);
}

@end
