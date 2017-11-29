#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"

#include "cv_bridge/cv_bridge.h"
#include "sensor_msgs/image_encodings.h"
#include "sensor_msgs/Image.h"


#include "image_transport/image_transport.h"

#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <time.h>

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

struct cameraObject{
	std::string ip;
	std::string port;
	std::string user;
	std::string pass;
	std::string fullURL;
};

int main(int argc, char** argv){
	ros::init(argc, argv, "network_streamer");
	ros::NodeHandle nh;

	std::string ip, port, user, pass, ns, show_image;

	image_transport::ImageTransport it(nh);
	image_transport::Publisher imagePublisher = it.advertise("image", 1);

	ns = ros::this_node::getNamespace();

	ROS_INFO("Resolved namespace: %s", ns.c_str());

	nh.param<std::string>("show_image", show_image, "true");
	nh.param<std::string>("ip", ip, "http://192.168.1.40");
	nh.param<std::string>("port", port, "81");
	nh.param<std::string>("user", user, "admin");
	nh.param<std::string>("pass", pass, "12345");

	ROS_INFO("IP: %s, Port: %s, Username: %s, Pass: %s, Show Image: %s", ip.c_str(), port.c_str(), user.c_str(), pass.c_str(), show_image.c_str());

  cameraObject networkCamera;
  networkCamera.ip = ip;
  networkCamera.port = port;
	networkCamera.user = user;
	networkCamera.pass = pass;

  networkCamera.fullURL = networkCamera.ip + ":" + networkCamera.port + "/videostream.cgi?loginuse=" + user + "&loginpas=" + pass + "&dummy=param.mjpg";
	ROS_INFO("URL to stream from: %s", networkCamera.fullURL.c_str());
	if(show_image == "true")
		cv::namedWindow("Network Stream",1);

  cv::VideoCapture capture(networkCamera.fullURL);
	ROS_INFO("Capture device initialized");
  cv::Mat frame;
  cv_bridge::CvImage rosImage;

	rosImage.encoding = "bgr8";
	rosImage.header.stamp = ros::Time::now();
	rosImage.header.frame_id = "camera";
	ROS_INFO("ROS Image Initialized");

  while(ros::ok()){
  	capture.read(frame);
  	if(frame.empty()){
			ROS_INFO("Frame is empty");
  		continue;
  	}
  	else{
  		rosImage.image = frame.clone();
			imagePublisher.publish(rosImage.toImageMsg());
			if(show_image == "true"){
				cv::imshow("Network Stream",frame);
				if(cv::waitKey(30) >= 0)
					break;
			}
  	}
  	ros::spinOnce();
	}

	return 0;
}
