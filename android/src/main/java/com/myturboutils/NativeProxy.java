package com.myturboutils;

// all native calls are moved to this class
// just not to make mess in MyTurboUtilsModule
// c++ counterparts are in cpp-adapter.cpp

import android.util.Log;

import com.facebook.react.bridge.ReactContext;
import com.facebook.react.turbomodule.core.CallInvokerHolderImpl;

// see https://github.com/facebookincubator/fbjni/blob/master/docs/quickref.md
public class NativeProxy {

  // fbjni demo
  public static native double nativeSumSquares(double a, double b);
  public static native String nativeMakeGreeting(String name);

  /*** jsi stuff ***/          //these could be static as well
  private native void installNativeJsi(long jsContextNativePointer, CallInvokerHolderImpl jsCallInvokerHolder);

  public void installJsi(ReactContext context) {
    Log.d("Turboutils", "Installing native...");
    CallInvokerHolderImpl holder = (CallInvokerHolderImpl)context.getCatalystInstance().getJSCallInvokerHolder();
    long contextPointer = context.getJavaScriptContextHolder().get();
    installNativeJsi(contextPointer, holder);
  }
}
