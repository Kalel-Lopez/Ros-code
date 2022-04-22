#include <ros.h> 
#include <std_msgs/String.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Twist.h>

const int pin = 2;
const int led = 6;

double distance = 0;

ros::NodeHandle nh;
geometry_msgs::Twist msg;
std_msgs::Float64 db_msg;

ros::Publisher pub("move", &msg);

void subscriberCallback(const std_msgs::Float64 &db_msg){
  distance = db_msg.data;
  if (distance > 15){
    analogWrite(led, 255);
  }
}

ros::Subscriber<std_msgs::Float64> sub("distance", &subscriberCallback);

void setup() {
  // put your setup code here, to run once:
  pinMode(pin, INPUT);
  pinMode(led, OUTPUT);
  digitalWrite(pin, HIGH);
  nh.initNode();//initialize node
  nh.advertise(pub); //start the publisher
  nh.subscribe(sub); //start the subscriber 

}

void loop() {
  // put your main code here, to run repeatedly:
  msg.linear.x = analogRead(A0);
  msg.angular.z = analogRead(A1);
  pub.publish(&msg);
  nh.spinOnce();
  
}
