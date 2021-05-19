#import <React/RCTBridgeModule.h>

@interface MyTurboUtils : NSObject <RCTBridgeModule>

@property(nonatomic, assign) BOOL setBridgeOnMainQueue;

@end
