package com.myturboutils

import com.facebook.react.ReactPackage
import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.uimanager.ViewManager

class MyTurboUtilsPackage : ReactPackage {
  override fun createNativeModules(reactContext: ReactApplicationContext)
    = listOf(MyTurboUtilsModule(reactContext))

  override fun createViewManagers(reactContext: ReactApplicationContext): List<ViewManager<*, *>>
    = emptyList()
}
