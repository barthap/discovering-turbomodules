import { NativeModules } from 'react-native';

type MyTurboUtilsBridgeType = {
  sumSquares(a: number, b: number): Promise<number>;
  makeGreetingFor(name: string): Promise<string>;
};

const { MyTurboUtils } = NativeModules;

export default MyTurboUtils as MyTurboUtilsBridgeType;
