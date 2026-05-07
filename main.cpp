#include <Arduino.h>

// Motor A
int motor1Pin1 = 25; 
int motor1Pin2 = 26; 
int motor2Pin1 = 27;
int motor2Pin2 = 14;

void ControlarMotores(int a){

  if (a<0){
    // Move DC motor backwards at maximum speed
    Serial.println("Moving Backwards");
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW); 

    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW); 

  } else if(a>0){
  // Move the DC motor forward at maximum speed
  Serial.println("Moving Forward");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);

  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  } else{
  // Stop the DC motor
  Serial.println("Motor stopped");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);

  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  }

}

void setup() {
  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  
  Serial.begin(115200);
}

void loop() {
  ControlarMotores(1);
  delay(2000);
  ControlarMotores(-1);
  delay(2000);
  ControlarMotores(0);
  delay(2000);
}