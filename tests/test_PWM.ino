// the number of the LED pin
const int ledPin = 2;  // 16 corresponds to GPIO16

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
 
void setup(){
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
}
 
void loop(){
  // increase the LED brightness
// changing the LED brightness with PWM

ledcWrite(ledChannel, 127);
delay(3000);

ledcWrite(ledChannel, 0);
delay(3000);
}