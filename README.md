# Discovering Turbo Modules

A journey to discover React Native TurboModules. Alghough TurboModules aren't officially released yet and there is almost no documentation, there are already some libraries based on new infrastructure, including awesome _[Reanimated 2](https://docs.swmansion.com/react-native-reanimated/)_ by SWM. I digged through React Native source code and various Github repositories to learn how it is done. I managed to get it working, so I share my step-by-step journey here.

[Each commit](https://github.com/barthap/discovering-turbomodules/commits/main) is a different stage towards a fully featured Turbo Module.

0. [see commit](https://github.com/barthap/discovering-turbomodules/commit/56cce3721b545818650bc139f49a04cbb19256a8) - Basic initialization and boilerplate (use `npx create-react-native-library` with C++ template)
1. [see commit](https://github.com/barthap/discovering-turbomodules/commit/db8243349dbf15698911bfe45af159a915b0b01d) - Adding `fbjni` for Android - it greatly improves Java - C++ interop.
2. [see commit](https://github.com/barthap/discovering-turbomodules/commit/592ec39f7c495790e87d51424f296205026e3919) - Migrating from legacy RN bridge to JSI.
3. [see commit](https://github.com/barthap/discovering-turbomodules/commit/5244ab8a6bd4f6688d8aff1413ee11b0da34b3e3) - Implementing the "Real" Turbo Module.
4. [see commit](https://github.com/barthap/discovering-turbomodules/commit/ed4adec1458e79a271a3145151824bf68fdf0689) - Calling Kotlin/Swift code from C++ module.
5. _TODO:_ Multithreading / asynchronous operations

I'm not so creative in examples, I ended up with something simple, but demonstrating:
The module consists of two methods:

- `sumSquares(a: number, b: number): number` - calculates `a^2 + b^2` - this method has C++ only implementation
- `makeGreetingFor(name: string): string` - returns _[Tag] Hello, {name}!_ - we want this method to be implemented for each platform separately in Kotlin/Swift.

## Useful repositories and articles about Turbo Modules:

- [Reanimated 2](https://github.com/software-mansion/react-native-reanimated)
- [react-native-multithreading](https://github.com/mrousavy/react-native-multithreading) - contains a subset of Reanimated 2 library - may be easier to understand
- [react-native-quick-sqlite](https://github.com/ospfranco/react-native-quick-sqlite) - JSI based C++ library for sqlite.
- [karol-bisztyga/rnfbjni](https://github.com/karol-bisztyga/rnfbjni) - Android only, probably a minimal TurboModule example
- _[How to create a react-native JSI module](https://ospfranco.github.io/post/2021/02/24/how-to-create-a-javascript-jsi-module/)_

## Known issues

- In iOS project, after each `pod install` you have to manually add `GreetingManager.swift` to project in Xcode. Adding `*.swift` to podfile sources causes conflicts with React dependencies, because they're not modularized. You could also try with `use_frameworks!`.

---

Below is a `react-native-builder-bob` generated readme:

## Installation

```sh
npm install my-turbo-utils
```

## Usage

```js
import MyTurboUtils from 'my-turbo-utils';

// 3*3 + 4*4 = 25
const result = await MyTurboUtils.sumSquares(3, 7);

const greeting = await MyTurboUtils.makeGreetingFor('TurboModules');
```

## Contributing

See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.

## License

MIT
