#include <opencv2/imgcodecs.hpp> 
#include <opencv2/highgui.hpp> 
#include <opencv2/imgproc.hpp> 
#include <iostream>

using namespace std;
using namespace cv;

Mat imgHSV, mask;
int hmin = 0, smin = 0, vmin = 0;
int hmax = 179, smax = 255, vmax = 255;//初始化各项参数
/// ///////////CHAPTER 6/////////////////

 /********  Color Detection  ***********/
void main()
{
	string path = "Resources/fan2.jpg";
	Mat img = imread(path);
	Mat imgResize;
	namedWindow("Image", WINDOW_NORMAL);
	namedWindow("ImageMASK", WINDOW_NORMAL);
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	namedWindow("Trackbars", (640, 200));
	createTrackbar("Hue Min", "Trackbars", &hmin, 179);
	createTrackbar("Hue Max", "Trackbars", &hmax, 179);
	createTrackbar("Sat Min", "Trackbars", &smin, 255);
	createTrackbar("Sat Max", "Trackbars", &smax, 255);
	createTrackbar("Val Min", "Trackbars", &vmin, 255);
	createTrackbar("Val Max", "Trackbars", &vmax, 255);
	//resize(img, imgResize, Size(), 0.5, 0.5);
	while (1)
	{
		Scalar lower(hmin, smin, vmin);//内含四个
		Scalar upper(hmax, smax, vmax);
		inRange(imgHSV, lower, upper, mask);

		imshow("Image", img);
		//imshow("ImageHSV", imgHSV);
		imshow("ImageMASK", mask);
		waitKey(1);
	}
	int area = contourArea(imgResize);
	cout << "Area:"<< area << endl;
}
