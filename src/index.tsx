import { NativeModules } from 'react-native';

type MyTurboUtilsType = {
  multiply(a: number, b: number): Promise<number>;
};

const { MyTurboUtils } = NativeModules;

export default MyTurboUtils as MyTurboUtilsType;
