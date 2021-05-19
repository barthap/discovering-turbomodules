package com.myturboutils

import com.facebook.react.bridge.Promise
import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.bridge.ReactContextBaseJavaModule
import com.facebook.react.bridge.ReactMethod
import com.facebook.react.module.annotations.ReactModule

@ReactModule(name = MyTurboUtilsModule.NAME)
class MyTurboUtilsModule(reactContext: ReactApplicationContext?) : ReactContextBaseJavaModule(reactContext) {
  companion object {
    const val NAME = "MyTurboUtils"

    init {
      try {
        // Used to load the 'my_turbo_utils' library on application startup.
        System.loadLibrary("my_turbo_utils")
      } catch (ignored: Exception) {
      }
    }
  }

  override fun getName() = NAME

  /**************** LEGACY BRIDGE METHODS  */

  @ReactMethod
  fun sumSquares(a: Double, b: Double, promise: Promise) {
    promise.resolve(NativeProxy.nativeSumSquares(a, b))
  }

  @ReactMethod
  fun makeGreetingFor(name: String, promise: Promise) {
    promise.resolve(NativeProxy.nativeMakeGreeting(name))
  }
}
