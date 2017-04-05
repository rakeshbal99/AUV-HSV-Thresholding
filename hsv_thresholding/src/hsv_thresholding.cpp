#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;

Mat frame;
int hue_low_default=0;
int hue_high_default=0;
int saturation_low_default=0;
int saturation_high_default=0;
int value_low_default=0;
int value_high_default=0;

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
  {

    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
    imshow("Image window", cv_ptr->image);
    inRange(cv_ptr->image,Scalar(hue_low_default,saturation_low_default,value_low_default), Scalar(hue_high_default,saturation_high_default,value_high_default),frame);
    imshow("Thresholded_Window",frame);
    waitKey(1);

}
int main(int argc, char** argv)
{
    ros::init(argc, argv, "hsv_thresholding_node");
    ros::NodeHandle hsv ;
    cout<<"Node Handle created\n";
    image_transport::ImageTransport transport_image(hsv);
    cout<<"Create 6 taskbars\n";   
    namedWindow( "Image window", WINDOW_AUTOSIZE );
    createTrackbar( "h_low", "Image window", &hue_low_default, 179);
    createTrackbar( "h_high", "Image window", &hue_high_default, 179);
    createTrackbar( "s_low", "Image window", &saturation_low_default, 255);
    createTrackbar( "s_high", "Image window", &saturation_high_default, 255);
    createTrackbar( "v_low", "Image window", &value_low_default, 255);
    createTrackbar( "v_high", "Image window", &value_high_default, 255);
    image_transport::Subscriber sub = transport_image.subscribe("/kraken/front_camera", 1, imageCallback);
    image_transport::Publisher pub = transport_image.advertise("/image_converter/output_video",1);
    ros::spin();
    return 0;

}
