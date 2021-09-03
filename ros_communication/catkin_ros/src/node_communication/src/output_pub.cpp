#include <ros/ros.h>
#include"node_communication/person.h"

int main(int argc, char**argv)
{
    
    ros::init(argc, argv, "output_pub");

    ros::NodeHandle n;

    ros::Publisher person_info_pub = n.advertise<node_communication::person>("/person_info",10);

    ros::Rate loop_rate(1);

    int count = 0;
    while(ros::ok())
    {
        // 初始化node_communication::person类型的消息
        node_communication::person output_msg;
        output_msg.a = 2;
        output_msg.b = 3;
        output_msg.sum = 0;

       person_info_pub.publish(output_msg);

       ROS_INFO("Publish Person Info: Number a:%d  ;Number b:%d", output_msg.a, output_msg.b);

       loop_rate.sleep();
    }
    return 0;
}