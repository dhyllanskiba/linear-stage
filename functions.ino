void count(){
  // counts number of pulses received from encoder
  if(digitalRead(7)==HIGH) pulses++;
  else pulses--;
}

void DC_Step(int steps, int s, int dir){
  // Turn the motor a given number of steps in a given direction
  // steps = number of steps
  // s = speed (0-255)
  // dir = direction of rotation (1 -> left; 0 -> right)

  // Delclare step counting variables
  long int initial_pulses=0, prev_pulses=0;
  
  // declare slow ramping variables
  unsigned char v = 0;
  float a = 0;

  // set the ramping initial position if there are more than 500 steps taken
  // scale up 'a' depending on the starting speed
  if (steps >= 500) a = (steps - (500 * (s/127.0))) /steps;


  // Turn motor on
  digitalWrite(12, dir);
  
  // build momentum by applying 5V briefly
  analogWrite(3,255);
  delay(10);
  analogWrite(3,s);
  
  // declare pulse variables for feedback
  initial_pulses = pulses;
  prev_pulses = pulses;

  // keep going until reached the number of pulses desired
  while(abs(pulses-initial_pulses)<steps){
    // start ramping down speed when a * steps is reached
    if (abs(pulses-initial_pulses)>a*steps){
      
      // linearly ramp down the voltage to 0 V
      // if the motor is no longer moving, increase the voltage
      if(pulses == prev_pulses) v = v+5;
      else v = (s)*(1-(abs(float(pulses-initial_pulses)/steps) - a)/(1-a));
      
      // set the voltage
      analogWrite(3,v);
      
      // set feedback pulses
      prev_pulses = pulses;
    }
      // print the time and number of pulses for plotting
      Serial.println(String(micros()) + "\t" + String(pulses));
      delay(1);
  }

  // Afterwards stop
  digitalWrite(3,LOW);  
}

void DC_Homing(int s, int dir){
  
  // Turn motor on
  digitalWrite(12, dir);
  
  // build momentum by applying 5V briefly
  analogWrite(3,255);
  delay(10);
      
  analogWrite(3,s);
  
  // keep going until button is pushed
  while(digitalRead(10)){
      Serial.println(String(micros()) + "\t" + String(pulses));
      delay(1);
  }
  
  // afterwards stop
  analogWrite(3,LOW);

}

void DC_SpeedTest(){
  // set initial time and pulses
  long int p0 = pulses;
  unsigned long int t0 = micros();
  
  // turn on motor
  digitalWrite(12, 0);

  // build momentum by applying 5V briefly
  analogWrite(3,255);
  delay(10);

  // give enough time to reach desired starting speed
  analogWrite(3,50);
  delay(100);

  // test the speed at different voltage levels
  for (int s = 50; s <= 255; s += 20){
      analogWrite(3,s);
      for(int i = 0; i < 100; i++){
        // print time and pulses for plotting
        Serial.println(String(micros()-t0) + "\t" + String(pulses-p0));
        delay(100);
      }
  }
  
  // afterwards stop
  analogWrite(3,0);

}
