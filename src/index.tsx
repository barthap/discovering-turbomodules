import { NativeModules } from 'react-native';

// Bridge
interface MyTurboUtilsBridged {
  sumSquares(a: number, b: number): Promise<number>;
  makeGreetingFor(name: string): Promise<string>;
}

export const { MyTurboUtils: MyUtilsBridged } = NativeModules as {
  MyTurboUtils: MyTurboUtilsBridged;
};

// JSI
interface MyTurboUtilsJSI {
  // notice it's synchronous
  sumSquares(a: number, b: number): number;
  makeGreetingFor(name: string): string;
}

// we can get global variable defined in C++
declare var _jsiTurboUtils: MyTurboUtilsJSI;
export const MyUtilsJSI = _jsiTurboUtils;
