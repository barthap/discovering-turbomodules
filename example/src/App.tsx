import * as React from 'react';

import { StyleSheet, View, Text } from 'react-native';
import { MyUtilsTurbo /* MyUtilsJSI,  MyUtilsBridged */ } from 'my-turbo-utils';

export default function App() {
  const [result, setResult] = React.useState<number | undefined>();
  const [greeting, setGreeting] = React.useState('none yet');

  React.useEffect(() => {
    // Bridged
    // MyUtilsBridged.sumSquares(3, 4).then(setResult);
    // MyUtilsBridged.makeGreetingFor('Bridge').then(setGreeting);
    setResult(MyUtilsTurbo.sumSquares(3, 4));
    setGreeting(MyUtilsTurbo.makeGreetingFor('TurboModule'));
  }, []);

  return (
    <View style={styles.container}>
      <Text>Sum Squares result: {result}</Text>
      <Text>Greeting: {greeting}</Text>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
  box: {
    width: 60,
    height: 60,
    marginVertical: 20,
  },
});
