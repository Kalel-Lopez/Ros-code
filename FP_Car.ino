#include <ros.h> 
#include <std_msgs/String.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Twist.h>
#include<Servo.h>
Servo servo;

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11

#define MAX_SPEED 255

int vf = 0;
int vr = 0;

std_msgs::Float64 db_msg;
ros::NodeHandle nh;
geometry_msgs::Twist msg;

double getDistance(){
  digitalWrite(A5, LOW);
  delayMicroseconds(2);
  digitalWrite(A5, HIGH);
  delayMicroseconds(10);
  digitalWrite(A5, LOW);
  
  long raw = pulseIn(A4, HIGH);
  double distance = raw * 0.034 / 2;

  return distance;
}
void stop(){
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
void leftSideMove(int s){
  analogWrite(ENA, abs(s));
  if (s>0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }
  if(s<0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
}
void rightSideMove(int s){
  analogWrite(ENB, abs(s));
  if (s>0) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  if(s<0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
}
void turnLeft(){
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnRight(){
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void obstacle(){
  float d = getDistance();
  if (d < 15) {
    stop();
    servo.write(180);
    delay(400);
    float a = getDistance();
    if (a > 15) {
      turnLeft();
      delay(1000);
      servo.write(90);
      stop();
    }
    else{
      servo.write(0);
      delay(400);
      float c = getDistance();
      if (c > 15) {
        turnRight();
        delay(1000);
        stop();
        servo.write(90);
      }
      else {
        turnRight();
        delay(2000);
        stop();
        servo.write(90);
      }
    }
  }
}

void chassisMove(int vf, int vr) {
 int leftSpeed = vf + vr;
 int rightSpeed = vf - vr;
 leftSideMove(leftSpeed);
 rightSideMove(rightSpeed);
}

ros::Publisher pub("distance", &db_msg);
ros::Subscriber<geometry_msgs::Twist> sub("move", &subscriberCallback);

void subscriberCallback(const geometry_msgs::Twist& msg){
  //content
  if msg.data {
    obstacle();
  }
  vr = msg.linear.x;
  vf = msg.angular.z;
  chassisMove(vf, vr);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(A4, INPUT);
  servo.attach(3);
  nh.initNode();//initialize node
  nh.advertise(pub); //start the publisher
  nh.subscribe(sub); //start the subscriber
}

void loop() {
  // put your main code here, to run repeatedly:
  db_msg.data = getDistance();
  pub.publish(&db_msg);
  nh.spinOnce(); //run node handler once to handle comms
}
