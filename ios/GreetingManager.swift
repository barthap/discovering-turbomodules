//
//  GreetingManager.swift
//  my-turbo-utils
//
//  Created by Bartłomiej Klocek on 19/05/2021.
//

import Foundation

@objc
class GreetingManager : NSObject {
  private var _tag: String;
  
  @objc
  init(withTag tag: String) {
    self._tag = tag
  }
  
  @objc
  func makeGreeting(forName name: String) -> String {
    return "[\(_tag)] Hello, \(name)!"
  }
}
