#include <ros.h> 
#include <std_msgs/String.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Bool.h>

#define D5 587

const int buzzer = 3;
const int sw = 2;

double distance = 0;

ros::NodeHandle nh;
geometry_msgs::Twist msg;
std_msgs::Float64 db_msg;
std_msgs::UInt16 b_msg;

ros::Publisher pub("move", &msg);
ros::Publisher pub2("button", &b_msg);

void subscriberCallback(const std_msgs::Float64 &db_msg){
  distance = db_msg.data;
  if (distance < 30.0){
    if (distance < 15.0){
      tone(buzzer, D5, 700/4);
      delay(50);
      noTone(buzzer);
      delay(10);
    }
    tone(buzzer, D5, 700/4);
    delay(250);
    noTone(buzzer);
    delay(10);
  }
  else {
    noTone(buzzer);
  }
}

ros::Subscriber<std_msgs::Float64> sub("distance", &subscriberCallback);

void setup() {
  // put your setup code here, to run once:
  pinMode(buzzer, OUTPUT);
  pinMode(sw, INPUT_PULLUP);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  nh.initNode();//initialize node
  nh.advertise(pub); //start the publisher
  nh.advertise(pub2);
  nh.subscribe(sub); //start the subscriber 

}

void loop() {
  // put your main code here, to run repeatedly:
  int x = analogRead(A0);
  x = map(x, 0, 1023, -255, 255);
  msg.linear.x = x;
  int y = analogRead(A1);
  y = map(y, 0, 1023, -255, 255);
  msg.linear.y = y;
  int button = digitalRead(2);
  if (button == HIGH){
    b_msg.data = 0;
  }
  else {
    b_msg.data = 1;
  }
  
  pub.publish(&msg);
  pub2.publish(&b_msg);
  nh.spinOnce();
  
}
