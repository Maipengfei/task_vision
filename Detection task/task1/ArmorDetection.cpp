#include <opencv2/imgcodecs.hpp> 
#include <opencv2/highgui.hpp> 
#include <opencv2/imgproc.hpp> 
#include <iostream>

using namespace std;
using namespace cv;

/// ///////////识别单个装甲板/////////////////

Mat imgHSV, mask, imgCanny, imgDil;
int hmin = 0, smin = 68, vmin = 56;
int hmax = 34, smax = 255, vmax = 255;//初始化各项参数


Point countInters(Point point1, Point point2, Point point3, Point point4 )
{
	Point Intersection;
	float x1 = point1.x, y1 = point1.y,
		x2 = point2.x, y2 = point2.y,
		x3 = point3.x, y3 = point3.y,
		x4 = point4.x, y4 = point4.y;
	//cout << x1<<";"<< x2 << ";" <<x3 << ";" <<x4 << ";" << y1 << ";" << y2 << ";" <<y3 << ";" << y4 << endl;
	
	float k1 = (y1 - y4) / (x1 - x4),
			k2 = (y2 - y3) / (x2 - x3);// 此处注意不能用整型变量，否则会被约为0
	//cout << "slope"<< (float)k1 << ";" << (float)k2 << endl;

	int x = (y2 - y1 + k1 * x1 - k2 * x2) / (k1 - k2);
	int y = (k1 * k2 * (x1 - x2) + k1 * y2 - k2 * y1) / (k1 - k2);
	/*Intersection.x = x;
	Intersection.y = y;*/
	return Point(x,y);
}

void armorDetection(Mat img)
{
	/********     颜色识别，转化为HSV图    ***********/
	cvtColor(img, imgHSV, COLOR_BGR2HSV);//
	Scalar lower(hmin, smin, vmin);//定义hue、saturation、value三个维度共六个参数
	Scalar upper(hmax, smax, vmax);
	inRange(imgHSV, lower, upper, mask);

	/********    contours    ***********/
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy(contours.size());

	Canny(mask, imgCanny, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));//尺寸越大，则膨胀越多
	dilate(imgCanny, imgDil, kernel);

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);


	vector<Rect> boundRect(contours.size());//不限制可能导致溢出???
	int n = 0;//计算灯柱
	//cout << "轮廓数" << contours.size() << endl;
	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		//cout << area << endl;
		if (area > 500)//根据图像面积过滤噪音
		{

			boundRect[n] = boundingRect(contours[i]);
			//drawContours(img, contours, i, Scalar(255, 0, 255), 2);
			//rectangle(img, boundRect[n].tl(), boundRect[n].br(), Scalar(0, 255, 0), 3);
			n++;
		}

	}
	line(img, Point(boundRect[0].x + boundRect[0].width / 2, boundRect[0].y),
		Point(boundRect[1].x + boundRect[1].width / 2, boundRect[1].y + boundRect[1].height), Scalar(255, 0, 0), 2);
	line(img, Point(boundRect[1].x + boundRect[1].width / 2, boundRect[1].y),
		Point(boundRect[0].x + boundRect[0].width / 2, boundRect[0].y + boundRect[0].height), Scalar(255, 0, 0), 2);

	circle(img, countInters(Point(boundRect[0].x + boundRect[0].width / 2, boundRect[0].y),
		Point(boundRect[1].x + boundRect[1].width / 2, boundRect[1].y),
		Point(boundRect[0].x + boundRect[0].width / 2, boundRect[0].y + boundRect[0].height),
		Point(boundRect[1].x + boundRect[1].width / 2, boundRect[1].y + boundRect[1].height)),
		5,
		Scalar(0, 255, 0), FILLED);


	imshow("Image", img);
	//imshow("ImageMASK", mask);
	//imshow("Image Contours", imgDil);
	waitKey(30);
}


 /********     Image    ***********/
int main()
{
	string path = "./Resources/fundamentaltask1.avi";
	VideoCapture box(path);
		if (box.isOpened())
		{
			cout <<"Great success"<< endl;
		}
		
		Mat img;
		int frame = 4, max;
		max = box.get(CAP_PROP_FRAME_COUNT);
		cout << box.get(CAP_PROP_FRAME_COUNT) << endl;
		while (frame < max)
		{
	
			box >> img;
			armorDetection(img);
			//imshow("img", img);
			frame++;

			//waitKey(8);
		}
		return 0;
	
	
	


}
