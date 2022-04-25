#include <ros.h> 
#include <std_msgs/String.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Twist.h>

const int led = 3;

double distance = 0;

ros::NodeHandle nh;
geometry_msgs::Twist msg;
std_msgs::Float64 db_msg;

ros::Publisher pub("move", &msg);

void subscriberCallback(const std_msgs::Float64 &db_msg){
  distance = db_msg.data;
  if (distance < 15.0){
    digitalWrite(led, HIGH);
  }
  else {
    digitalWrite(led, LOW);
  }
}

ros::Subscriber<std_msgs::Float64> sub("distance", &subscriberCallback);

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  nh.initNode();//initialize node
  nh.advertise(pub); //start the publisher
  nh.subscribe(sub); //start the subscriber 

}

void loop() {
  // put your main code here, to run repeatedly:
  int x = analogRead(A0);
  x = map(x, 0, 1023, -255, 255);
  msg.linear.x = x;
  int z = analogRead(A1);
  z = map(z, 0, 1023, -255, 255);
  msg.angular.z = z;
  pub.publish(&msg);
  nh.spinOnce();
  
}
