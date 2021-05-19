package com.myturboutils;

// all native calls are moved to this class
// just not to make mess in MyTurboUtilsModule
// c++ counterparts are in cpp-adapter.cpp

// see https://github.com/facebookincubator/fbjni/blob/master/docs/quickref.md
public class NativeProxy {

  public static native double nativeSumSquares(double a, double b);
  public static native String nativeMakeGreeting(String name);
}
