package com.myturboutils

import com.facebook.react.bridge.Promise
import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.bridge.ReactContextBaseJavaModule
import com.facebook.react.bridge.ReactMethod
import com.facebook.react.module.annotations.ReactModule
import com.myturboutils.MyTurboUtilsModule

@ReactModule(name = MyTurboUtilsModule.NAME)
class MyTurboUtilsModule(reactContext: ReactApplicationContext?) : ReactContextBaseJavaModule(reactContext) {
  override fun getName(): String {
    return NAME
  }

  companion object {
    const val NAME = "MyTurboUtils"
    external fun nativeSumSquares(a: Double, b: Double): Double

    init {
      try {
        // Used to load the 'native-lib' library on application startup.
        System.loadLibrary("cpp")
      } catch (ignored: Exception) {
      }
    }
  }

  /**************** LEGACY BRIDGE METHODS  */ // Example method
  // See https://reactnative.dev/docs/native-modules-android
  @ReactMethod
  fun sumSquares(a: Double, b: Double, promise: Promise) {
    promise.resolve(nativeSumSquares(a, b))
  }

  @ReactMethod
  fun makeGreetingFor(name: String, promise: Promise) {
    promise.resolve("[Java Bridge] Hello, $name!")
  }
}
