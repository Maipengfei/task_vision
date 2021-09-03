#include <opencv2/imgcodecs.hpp> 
#include <opencv2/highgui.hpp> 
#include <opencv2/imgproc.hpp> 
#include <iostream>

using namespace std;
using namespace cv;

/// ///////////识别单个装甲板/////////////////

Mat imgHSV, mask, imgCanny, imgDil;
int hmin = 0, smin = 165, vmin = 101;
int hmax = 15, smax = 255, vmax = 255;//初始化各项参数

/******************************************************************
* Point countInters(Point point1, Point point2, Point point3, Point point4) 
*@brief：计算打击点坐标
*@param：分别对应矩形从top-right点开始，顺时针四个点坐标
*******************************************************************/
Point countInters(Point point1, Point point2, Point point3, Point point4)
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
	return Point(x, y);
}


/******************************************************************
* @mpf
* @brief：识别装甲板，并标出中心点
* @param：图像
*******************************************************************/
void armorDetection(Mat img)
{
	/********     颜色识别，转化为二值图    ***********/
	cvtColor(img, imgHSV, COLOR_BGR2HSV);
	//将原图转化为HSV图
	Scalar lower(hmin, smin, vmin);//
	Scalar upper(hmax, smax, vmax);

	inRange(imgHSV, lower, upper, mask);
	namedWindow("Image", WINDOW_NORMAL);
	namedWindow("ImageMASK", WINDOW_NORMAL);

	/********    contours    ***********/
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy(contours.size());

	Canny(mask, imgCanny, 25, 75);
	//轮廓检测
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));//尺寸越大，则膨胀越多
	dilate(imgCanny, imgDil, kernel);
   //膨胀轮廓
	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<Rect> boundRect(contours.size());
	//不限制可能导致溢出
	int n = 0;
	//用于计算灯柱

	//cout << "轮廓数" << contours.size() << endl;
	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		cout << area << endl;
		if (area > 210)//根据轮廓面积过滤噪音
		{

			boundRect[n] = boundingRect(contours[i]);
			drawContours(img, contours, i, Scalar(255, 0, 255), 2);
			rectangle(img, boundRect[n].tl(), boundRect[n].br(), Scalar(0, 255, 0), 3);
			int x = boundRect[n].x;
			cout << "x:" << x << endl;
			cout << "y:" << boundRect[n].height << endl;
			n++;
		}
	}

	//cout << "灯柱数：" << n << endl;
	int k = 0;
	//计算有效灯柱
	vector<Rect> realRect(contours.size());
	//通过两个for循环将灯柱相互匹配，并根据宽高比确定两条灯柱是否构成一个装甲板
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			//根据宽高比确定两条灯柱是否构成一个装甲板
			float width = boundRect[i].x - boundRect[j].x,
					height = (boundRect[i].height+ boundRect[j].height)/2,
				    ratioWH = abs(width/ height);

			cout << "Ratio:" << ratioWH << endl;
			if (ratioWH > 1.5 && ratioWH < 2.8)
			{
				rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 2);
				rectangle(img, boundRect[j].tl(), boundRect[j].br(), Scalar(0, 255, 0), 2);
				//将两个匹配灯柱分别对应在原图画上竖直最小矩形

				line(img, Point(boundRect[i].x + boundRect[i].width / 2, boundRect[i].y),
					Point(boundRect[j].x + boundRect[j].width / 2, boundRect[j].y + boundRect[j].height), Scalar(255, 0, 0), 2);
				line(img, Point(boundRect[j].x + boundRect[j].width / 2, boundRect[j].y),
					Point(boundRect[i].x + boundRect[i].width / 2, boundRect[i].y + boundRect[i].height), Scalar(255, 0, 0), 2);
				//根据两个矩形的上下宽的中点坐标，画对角线


				circle(img, countInters(Point(boundRect[i].x + boundRect[i].width / 2, boundRect[i].y),
					Point(boundRect[j].x + boundRect[j].width / 2, boundRect[j].y),
					Point(boundRect[i].x + boundRect[i].width / 2, boundRect[i].y + boundRect[i].height),
					Point(boundRect[j].x + boundRect[j].width / 2, boundRect[j].y + boundRect[j].height)),
					3,
					Scalar(0, 255, 0), FILLED);
					//调用countInters()函数得出两对角线的交点，进而画装甲板中心
			}
			
		}
	}

	imshow("Image", img);
	//imshow("ImageMASK", mask);
	//imshow("Image Contours", imgDil);
	waitKey(50);
}


/********     Image    ***********/
int main()
{
	
	string path = "./Resources/fundamentaltask2.avi";
	VideoCapture box(path);
	namedWindow("Image", WINDOW_NORMAL);
	if (box.isOpened())
	{
		cout << "Great success" << endl;
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

	}
	return 0;
}



