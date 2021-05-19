#import "MyTurboUtils.h"
#import "my_turbo_utils-Swift.h"

#include "TurboUtilsModule.h"

@implementation MyTurboUtils

RCT_EXPORT_MODULE()


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
