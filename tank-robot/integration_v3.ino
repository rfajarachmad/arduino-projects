#include <Servo.h>

#define trigPin 11
#define echoPin 12

Servo servo; // Define our Servo

const int IN1 = 7; //Left1
const int IN2 = 9; //Left2
const int IN3 = 8; //Right1
const int IN4 = 10; //Right2
const int TRASHOLD = 10;
const int GREEN_LED = 10;
long duration;
int leftdist, rightdist, distanceFwd;


void setup(){
  Serial.begin (9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  servo.attach(13);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(GREEN_LED, OUTPUT);
}

void loop(){
  distanceFwd = getDistance();
  Serial.print("Forward Distance : ");
  Serial.println(distanceFwd);
  stopEngine();
  delay(500);
  while(distanceFwd >= TRASHOLD){
    Serial.println("Move Forward");
    moveForward();
    distanceFwd = getDistance();
  }
  
  if(distanceFwd < TRASHOLD){
    stopEngine();
    Serial.println("Stop Engine");
    servo.write(0);
    delay(500);
    rightdist= getDistance();
    Serial.print("Right Distance : ");
    Serial.println(rightdist);
    delay(500);
    servo.write(180);
    delay(700);
    leftdist= getDistance();
    Serial.print("Left Distance : ");
    Serial.println(leftdist);
    delay(500);
    servo.write(90);
    delay(700);
  }
  
  while(distanceFwd < TRASHOLD*2){
    
    if(leftdist < rightdist){
      turnLeft();
      Serial.println("Turn Left");
    }else{
      turnRight();
      Serial.println("Turn Right");
    }
    
    distanceFwd = getDistance();
    Serial.print("Forward Distance : ");
    Serial.println(distanceFwd);
  }
  
}

void moveForward(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
    
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopEngine(){
 digitalWrite(IN1, LOW);
 digitalWrite(IN2, LOW);
 digitalWrite(IN3, LOW);
 digitalWrite(IN4, LOW);
}

long getDistance()
{
  long distance, duration, sum;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;  
  delay(100);
  return distance;
}

void turnLeft(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnRight(){
 digitalWrite(IN1, LOW);
 digitalWrite(IN2, HIGH);
 digitalWrite(IN3, HIGH);
 digitalWrite(IN4, LOW);
}

void compareDistance(){
  distanceFwd = getDistance();
  if (leftdist<rightdist){
    Serial.println("right"+distanceFwd);
    while(distanceFwd <= TRASHOLD){
      turnRight();
      distanceFwd = getDistance();
    }
  }else{
    Serial.println("left"+distanceFwd);
    while(distanceFwd <= TRASHOLD){
      turnLeft();
      distanceFwd = getDistance();
    }
  }
}

void turnOnLED(int ledColor){
  digitalWrite(ledColor, HIGH); 
}

void turnOffLED(int ledColor){
  digitalWrite(ledColor, LOW); 
}
