# Discovering Turbo Modules

A journey to discover RN TurboModules. Each commit is a different stage towards a fully featured Turbo Module.

0. Basic initialization and boilerplate (use `npx create-react-native-library` with C++ template)
1. Adding `fbjni` for Android - it greatly improves Java - C++ interop.
2. Migrating from legacy RN bridge to JSI.
3. _TODO:_ Implementing the "Real" Turbo Module.
4. _TODO:_ Calling Kotlin/Swift code from C++ module.
5. _TODO:_ Multithreading / asynchronous operations

I'm not so creative in examples, I ended up with something simple, but demonstrating:
The module consists of two methods:

- `sumSquares(a: number, b: number): number` - calculates `a^2 + b^2` - this method has C++ only implementation
- `makeGreetingFor(name: string): string` - returns _[Tag] Hello, {name}!_ - we want this method to be implemented for each platform separately in Kotlin/Swift.

# Useful repositories and articles about Turbo Modules:

- [Reanimated 2]()
- [react-native-multithreading](https://github.com/mrousavy/react-native-multithreading) - contains a subset of Reanimated 2 library - may be easier to understand
- [react-native-quick-sqlite](https://github.com/ospfranco/react-native-quick-sqlite) - JSI based C++ library for sqlite.
- [karol-bisztyga/rnfbjni](https://github.com/karol-bisztyga/rnfbjni) - Android only, probably a minimal TurboModule example

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
