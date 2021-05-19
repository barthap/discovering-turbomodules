package com.myturboutils

// this class is called from C++ (see cpp-adapter.cpp)
class GreetingManager(private val tag: String) {

  fun makeGreeting(name: String) = "[$tag] Hello, $name!"
}
