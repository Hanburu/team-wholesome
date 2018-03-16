 int joyPin_X = A0;                 // slider variable connecetd to analog pin 0
 int joyPin_Y = A1;                 // slider variable connecetd to analog pin 1
 int xMotorPin = 9;
 int yMotorPin = 10;
 int value_X = 0;                  // variable to read the value from the analog pin 0
 int value_Y = 0;                  // variable to read the value from the analog pin 1
 int targetPWM_X = 0;
 int currentPWM_X = 0;
 int targetPWM_Y = 0;
 int currentPWM_Y = 0;

 void setup() {
  pinMode(xMotorPin, OUTPUT);
  pinMode(yMotorPin, OUTPUT);
  Serial.begin(9600);
 }

 void loop() {
  // reads the value of the variable resistor 
  value_X = analogRead(joyPin_X);   
  // this small pause is needed between reading
  // analog pins, otherwise we get the same value twice
  //delay(100);             
  // reads the value of the variable resistor 
  value_Y = analogRead(joyPin_Y);


  // PWM Handling
  int targetPWM_X = map(value_X, 0, 1023, 50, 200);
  int targetPWM_Y = map(value_Y, 0, 1023, 50, 200);
  
  if ((value_X >= 500) && (value_X <= 520)){
    //
  }
  else{
    currentPWM_X += (targetPWM_X - currentPWM_X) ;
  }
  if ((value_Y >= 500) && (value_Y <= 520)){
    //
  }
  else{
    currentPWM_Y += (targetPWM_Y - currentPWM_Y) ;
  }
  analogWrite(xMotorPin, currentPWM_X);
  analogWrite(yMotorPin, currentPWM_Y);
  Serial.println("Current");
  Serial.println(currentPWM_X);
  Serial.println("Target");
  Serial.println(targetPWM_X);
  Serial.println(" ");
  delay(250); 
 }
  
