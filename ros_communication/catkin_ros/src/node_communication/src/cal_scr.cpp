/***********************************************************************
Copyright 2020 GuYueHome (www.guyuehome.com).
***********************************************************************/

/**
 * 该例程将订阅/person_info话题，自定义消息类型learning_topic::Person
 */
 
#include <ros/ros.h>
#include "node_communication/person.h"
 unsigned int sum;
// 接收到订阅的消息后，会进入消息回调函数
void personInfoCallback(const node_communication::person::ConstPtr&  msg)
{
    sum = msg->a+msg->b;
    // 将接收到的消息打印出来
    ROS_INFO("Subcribe Person Info: name: Number a:%d  ;Number b:%d  ; Sum: %d",msg->a,msg->b,sum);
}

int main(int argc, char **argv)
{
    // 初始化ROS节点
    ros::init(argc, argv, "cal_scr");

    // 创建节点句柄
    ros::NodeHandle n;
   
  
    // 创建一个Subscriber，订阅名为/person_info的topic，注册回调函数personInfoCallback
    ros::Subscriber person_info_sub = n.subscribe("/person_info", 10, personInfoCallback);

    // 循环等待回调函数
    ros::spin();

    return 0;
}

