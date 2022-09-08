void setup() {
  pinMode(7,INPUT);   // Encoder
  attachInterrupt(digitalPinToInterrupt(2),count,RISING);   // Interrupt
  Serial.begin(115200);

  pinMode(12,OUTPUT);   // Direction
  pinMode(3,OUTPUT);    // Voltage
  pinMode(9,OUTPUT);    // Brake

  pinMode(10,INPUT_PULLUP); // Button

  digitalWrite(9,LOW); // Turn off Brake
}

volatile long int pulses=0;

void loop() {
  // wait for button push to start
  while(digitalRead(10));
  
  // start 1 second after button press
  delay(1000);

  // approach the button at ~35 mm/s
  DC_Homing(130,0);

  // step away from the button 10 mm at ~20mm/s
  DC_Step(10*(374/8), 70, 1);

  // approach the button again at ~10 mm/s
  DC_Homing(60,0);
  
  // step away from the button 30 mm at ~10 mm/s
  DC_Step(30*(374/8), 60, 1);

  // done
  while(true);

}
