/* arduino-traffic-light.ino
 *
 * Copyright 2019 Oseias Rocha & Israel Cristiano Korndorfer
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
//#include <CurieTime.h>

// pinout
int c_r = 12;
int c_y = 11;
int c_g = 10;
int p_r = 9;
int p_g = 8;

// interrupt button pin 2
int int_button = 2;

// controls reduction time
int controller = 1;

// traffic lights times

// car red
long time_c_r = 10000;
// car yellow
long time_c_y = 5000;
// car green
long time_c_g = 15000;

// car green reduction time if button pressed
long time_button = 10000;

// pedestrian red blink time
long time_ped = 2500;

// auxiliary variables to calculate time
unsigned long prev_time = 0, now_time = 0;

void setup() {                
  // pin initialization
  pinMode(c_r, OUTPUT);
  pinMode(c_y, OUTPUT);
  pinMode(c_g, OUTPUT);
  pinMode(p_r, OUTPUT);
  pinMode(p_g, OUTPUT);
  pinMode(int_button, INPUT);

  // button interrurupt configuration
  attachInterrupt(digitalPinToInterrupt(int_button), decrease_time, RISING);
}

void decrease_time(){
  if(digitalRead(c_g) == HIGH && controller == 1){
    prev_time -= time_button;
    controller = 0;
  }
}

void loop() {
  if(now_time == 0){
    // initial disposition of lights
    digitalWrite(c_r, LOW);
    digitalWrite(c_g, HIGH);
    digitalWrite(c_y, LOW);
    digitalWrite(p_r, HIGH);
    digitalWrite(p_g, LOW);
  }else{
    if ((now_time - prev_time) >= time_c_g && digitalRead(c_g) == HIGH){
      // green to yellow (cars)
      digitalWrite(c_g, LOW);
      digitalWrite(c_y, HIGH);
      prev_time = now_time;
    }else if((now_time - prev_time) >= time_c_y && digitalRead(c_y) == HIGH){
      // yellow to red (cars)
      digitalWrite(c_y, LOW);
      digitalWrite(c_r, HIGH);
      delay(1000);
      // red to green (pedestrian)
      digitalWrite(p_r, LOW);
      digitalWrite(p_g, HIGH);
      prev_time = now_time;
    }else if((now_time - prev_time) >= time_c_r && digitalRead(c_r) == HIGH){
      // red (pedestrian)
      digitalWrite(p_r, HIGH);
      delay(1000);
      // red to green (cars)
      digitalWrite(c_r, LOW);
      digitalWrite(c_g, HIGH);
      controller = 1;
      prev_time = now_time;
    }else if((now_time - prev_time) >= (time_c_r - time_ped) && digitalRead(c_r) == HIGH){
      // green to red (pedestrian)
      digitalWrite(p_g, LOW);
      // blink red (pedestrian)
      if ((now_time - prev_time) % 500 == 0){
        digitalWrite(p_r, !digitalRead(p_r));
      }
    }
  }
  now_time = millis();
}
