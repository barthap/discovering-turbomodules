//
//  IosTurboUtilsAdapter.h
//  my-turbo-utils
//
//  Created by Bartłomiej Klocek on 19/05/2021.
//

#ifndef IosTurboUtilsAdapter_h
#define IosTurboUtilsAdapter_h

#include "PlatformAdapter.h"
#import "my_turbo_utils-Swift.h"

class IosAdapter : public turboutils::PlatformAdapter {
  GreetingManager* _greetingManager;
public:
  IosAdapter() {
    _greetingManager = [[GreetingManager alloc] initWithTag:@"iOS"];
  }
  virtual ~IosAdapter() {
    _greetingManager = nil;
  }

  std::string delegateGreeting(const std::string &name) override {
    // convert std::string to NSString*
    NSString* nsName = [[NSString alloc] initWithCString:name.c_str() encoding:NSUTF8StringEncoding];
    
    // call Swift method
    NSString* result = [_greetingManager makeGreetingForName:nsName];
    
    // convert back to std::string
    return std::string([result UTF8String]);
  }
};

#endif /* IosTurboUtilsAdapter_h */
