#include <ros.h> 
#include <std_msgs/String.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Twist.h>

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11

#define MAX_SPEED 255

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

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println("cm");
  
  return distance;
}

void leftSide Move(int s){
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

void chassisMove(int vf, int vr) {
 int leftSpeed = vf + vr;
 int rightSpeed = vf - vr;
 leftSideMove(leftSpeed);
 rightSideMove(rightSpeed);
}

ros::Publisher pub("distance", &db_msg);
ros::Subscriber<geometry_msgs::Twist> sub("teleop", &subscriberCallback);

void subscriberCallback(const geometry_msgs::Twist &msg){
  //content
  vf = msg.linear.x;
  vr = msg.angular.z;
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
