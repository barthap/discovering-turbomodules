import * as React from 'react';

import { StyleSheet, View, Text, TouchableOpacity } from 'react-native';
import { MyUtilsTurbo /* MyUtilsJSI,  MyUtilsBridged */ } from 'my-turbo-utils';

export default function App() {
  const [result, setResult] = React.useState<number | undefined>();
  const [greeting, setGreeting] = React.useState('none yet');
  const [asyncResult, setAsyncResult] = React.useState('Not started');

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
      <Button
        label="Run Async call"
        onPress={() => {
          console.log('before');
          setAsyncResult('Started, waiting...');
          MyUtilsTurbo.funAsync().then((result) => {
            console.log('Promise result:', result);
            setAsyncResult(result);
          });
          console.log('after');
        }}
      />
      <Text>Async result: {asyncResult}</Text>
    </View>
  );
}

interface ButtonProps {
  label: string;
  onPress: () => void;
}

const Button = ({ label, onPress }: ButtonProps) => (
  <TouchableOpacity style={styles.buttonContainer} onPress={onPress}>
    <Text style={styles.buttonText}>{label}</Text>
  </TouchableOpacity>
);

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
  buttonContainer: {
    backgroundColor: '#4630eb',
    borderRadius: 4,
    padding: 12,
    marginVertical: 10,
    justifyContent: 'center',
    alignItems: 'center',
  },
  buttonText: {
    color: '#ffffff',
    fontSize: 16,
  },
});
