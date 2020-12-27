#include "ros/ros.h"
#include "std_msgs/String.h"
#include <queue>
#include <sensor_msgs/Imu.h>
#include<geometry_msgs/Twist.h>
#include<nav_msgs/Odometry.h>

using namespace std;
using namespace sensor_msgs;
using namespace geometry_msgs;

string imu_topic = "/imu/data";
string odom_topic = "/cmd_vel";

//创建容器接受对象--------->虽然可以编译成功，但一运行产生 segmentation fault 错误....
//queue<sensor_msgs::Imu> imu_data_buf;
//sensor_msgs::Imu imu_data_raw;

//geometry_msgs::Twist twist_data_raw;
//queue<geometry_msgs::Twist>  twist_data_buf;

float x = 0.0;
float y  = 0.0;
float th = 0.0;

float vx = 0.0;
float vy = 0.0;
float vth = 0.0;

float w = 0.0;


void imu_callback(const sensor_msgs::Imu &imu_msg)
{
    //imu_data_buf.push(imu_msg);
    w = imu_msg.angular_velocity.z;

}

void odomCallBack(const geometry_msgs::Twist &twist_msg)
{
     //twist_data_buf.push(twist_msg);
     vx = twist_msg.linear.x;

}

/*void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
    ROS_INFO("I heard: [%s]", msg->data.c_str());
}*/

int main(int argc, char **argv){
    ros::init(argc, argv, "listener");
    ros::NodeHandle n;
    ros::Subscriber imu_sub = n.subscribe(imu_topic, 100, imu_callback);
    ros::Subscriber odom_sub = n.subscribe(odom_topic,100,odomCallBack);

    ros::Rate loop_rate(1);

    ros::Time current_time,last_time;
    current_time = ros::Time::now();
    last_time = ros::Time::now();

    while (ros::ok())
    {

       // imu_data_raw.header.stamp = ros::Time::now();
        //imu_data_raw = imu_data_buf.back();
        //imu_data_raw.header.frame_id = "imu_link";

        //float w = imu_data_raw.angular_velocity.z;

        //twist_data_raw = twist_data_buf.back();

        //vx = twist_data_raw.twist.twist.angular.z;


        current_time = ros::Time::now();
        float dt = (current_time - last_time).toSec();
        float delta_th = w * dt;
        float delta_x = cos(th + delta_th/2) * vx * dt;
        float delta_y = sin(th + delta_th/2) * vx * dt;

        th += delta_th;
        x += delta_x;
        y += delta_y;

        cout << "the current position is   ( " <<  x << " , " << y <<" )" << endl;

        //ROS_INFO("the current position is :  %lf",x, "  ,  %lf",y);
        //cout << "the current time is   ( " <<  current_time.toSec() <<" )" << endl;

        ros::spinOnce();
        loop_rate.sleep();
      

    }


    //ros::spin();
    return 0;
}
