#define pot_pin A0
#define pwm_pin 6
int pwm;
void setup() {
  Serial.begin(9600);
  pinMode(pwm_pin,OUTPUT);
}

void loop() {
  pwm=analogRead(A0);
  analogWrite(pwm_pin,map(pwm,0,1023,0,255));
  Serial.println(pwm);
  
}
