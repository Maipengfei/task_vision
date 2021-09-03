#include <opencv2/imgcodecs.hpp> 
#include <opencv2/highgui.hpp> 
#include <opencv2/imgproc.hpp> 
#include <opencv2/calib3d.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/// ///////////识别单个装甲板/////////////////

Mat imgHSV, mask, imgCanny, imgDil, imgDilmask;
int hmin = 11, smin = 0, vmin = 47;
int hmax = 152, smax = 255, vmax = 255;//初始化各项参数


float getDistance(Point pointO, Point pointA)//计算两点距离
{
	float distance;
	distance = powf((pointO.x - pointA.x), 2) + powf((pointO.y - pointA.y), 2);
	distance = sqrtf(distance);
	return distance;
}

vector<Point2f> reArange(Point2f Point[4])
{
	double width;
	double height;
	vector<Point2f> result;//注意不要在此限制尺寸，否则后面的backpush将会从第n+1个开始赋值
	//矫正提取的叶片的宽高
	width = getDistance(Point[0], Point[1]);
	height = getDistance(Point[1], Point[2]);
	if (width > height)
	{

		result.push_back(Point[1]);
		result.push_back(Point[2]);
		result.push_back(Point[3]);
		result.push_back(Point[0]);

		if (result[0].x > result[2].x)
		{
			swap(result[0], result[2]);
			swap(result[1], result[3]);
			cout << "swap sucessfully" << endl;
		}
		cout << "swap sucessfully" << endl;
		return result;

	}
	else
	{
		result.push_back(Point[0]);
		result.push_back(Point[1]);
		result.push_back(Point[2]);
		result.push_back(Point[3]);
		if (result[0].x > result[2].x)
		{
			swap(result[0], result[2]);
			swap(result[1], result[3]);
			cout << "swap sucessfully" << endl;
		}
		cout << "swap sucessfully" << endl;
		return result;
	}

}
Point countInters(Point point1, Point point2, Point point3, Point point4)//point1与point4相对
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



void armorDetection(Mat img)
{

	namedWindow("Image", WINDOW_NORMAL);
	namedWindow("ImageDil", WINDOW_NORMAL);
	//namedWindow("ImageDilmask", WINDOW_NORMAL);

	/********     颜色识别，转化为二值图    ***********/
	vector<Mat> imgChannels;
	split(img, imgChannels);
	//获得目标颜色图像的二值图
	Mat midImage = imgChannels.at(0) - imgChannels.at(2);//保留蓝色
	threshold(midImage, mask, 80, 255, THRESH_BINARY);

	//Mat midImage = imgChannels.at(0) - imgChannels.at(2);
	Mat kernel1 = getStructuringElement(MORPH_RECT, Size(3, 3));//尺寸越大，则膨胀越多
	dilate(mask, imgDilmask, kernel1);
	imshow("ImageMask", mask);
	//imshow("ImageDilmask", imgDilmask);
	/********    contours    ***********/
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	Canny(imgDilmask, imgCanny, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(1, 3));//尺寸越大，则膨胀越多
	dilate(imgCanny, imgDil, kernel);
	imshow("ImageDil", imgDil);
	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);


	vector<RotatedRect> boundRect(contours.size());//不限制可能导致溢出???
	Point2f P[4];//可能为4
	vector<Point2f> result;
	int n1 = 0;//计算灯柱
	int n = 0;
	vector<vector<Point2f>> minrectPoint;
	//cout << "轮廓数" << contours.size() << endl;


	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);

		if (area > 60 && area < 280)//根据图像面积过滤噪音
		{

			//boundRect[n] = boundingRect(contours[i]);
			boundRect[n1] = minAreaRect(contours[i]);
			boundRect[n1].points(P);
			minrectPoint.push_back(reArange(P));
			circle(img, Point(minrectPoint[n1][0]), 5, Scalar(255, 0, 0), FILLED);
			float width = getDistance(minrectPoint[n1][0], minrectPoint[n1][1]),
				height = getDistance(minrectPoint[n1][1], minrectPoint[n1][2]),
				ratioWH = abs(height / width);
			float slope = abs((minrectPoint[n1][1].y - minrectPoint[n1][2].y) / (minrectPoint[n1][1].x - minrectPoint[n1][2].x));
			//通过灯柱的高的斜率筛去无效灯柱
			cout << "slope" << slope << endl;
			if (ratioWH > 2.0 && ratioWH < 5.0 && height < 45&&slope > 0.5 )
			{
				//cout << "Height:" << height << endl;
				//cout << "Ratio:" << ratioWH << endl;
				//用于判断灯柱四个点顺序是否正确
				circle(img, Point(minrectPoint[n1][0]), 1, Scalar(255, 0, 0), FILLED);
				circle(img, Point(minrectPoint[n1][1]), 1, Scalar(0, 255, 0), FILLED);
				circle(img, Point(minrectPoint[n1][2]), 1, Scalar(0, 0, 255), FILLED);
				circle(img, Point(minrectPoint[n1][3]), 1, Scalar(255, 255, 0), FILLED);
				n++;
			}
			else
			{
				//删掉不符合条件的矩形点
				minrectPoint.pop_back();
				n1--;
			}
			n1++;

		}

	}
	cout << "value n:" << n << endl;
	//cout << "灯柱数：" << n << endl;
	
	int k = 0;//计算有效灯柱,即可以与另一灯柱匹配成装甲板
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{

			if (minrectPoint[i][0].x > minrectPoint[j][0].x)
			{
				swap(minrectPoint[i], minrectPoint[j]);

			}
			float width = getDistance(minrectPoint[i][0], minrectPoint[j][0]),
				height = getDistance(minrectPoint[i][0], minrectPoint[i][3]),
				ratioWH = abs(width / height);
			float pan = abs(minrectPoint[i][0].y - minrectPoint[j][3].y);
			
			if (ratioWH > 1.5 && ratioWH < 4.0&& pan > 20&&pan<35)
			{
				cout << "Pan:" << pan << endl;
	
				line(img, Point((minrectPoint[i][0] + minrectPoint[i][1]) / 2), Point((minrectPoint[j][2] + minrectPoint[j][3]) / 2), Scalar(0, 255, 0), 2);
				line(img, Point((minrectPoint[i][2] + minrectPoint[i][3]) / 2), Point((minrectPoint[j][0] + minrectPoint[j][1]) / 2), Scalar(0, 255, 0), 2);
				circle(img, countInters((minrectPoint[i][0] + minrectPoint[i][1]) / 2,
					(minrectPoint[j][0] + minrectPoint[j][1]) / 2,
					(minrectPoint[i][2] + minrectPoint[i][3]) / 2,
					(minrectPoint[j][2] + minrectPoint[j][3]) / 2),
					3, Scalar(0, 0, 255), FILLED);

			}

		}
	}


	/*line(img, Point((minrectPoint[0][0] + minrectPoint[0][1]) / 2), Point((minrectPoint[1][0] + minrectPoint[1][1]) / 2), Scalar(255, 0, 0), 2);
	line(img, Point((minrectPoint[1][0] + minrectPoint[1][1]) / 2), Point((minrectPoint[1][2] + minrectPoint[1][3]) / 2), Scalar(255, 0, 0), 2);
	line(img, Point((minrectPoint[1][2] + minrectPoint[1][3]) / 2), Point((minrectPoint[0][2] + minrectPoint[0][3]) / 2), Scalar(255, 0, 0), 2);
	line(img, Point((minrectPoint[0][2] + minrectPoint[0][3]) / 2), Point((minrectPoint[0][0] + minrectPoint[0][1]) / 2), Scalar(255, 0, 0), 2);*/


}


/********     Image    ***********/
void main()
{

	/*string path = "Resources/RM2021_3.png";
	Mat img = imread(path);
	imshow("Image", img);
	armorDetection(img);*/
	string path = "Resources/RM2021_1.avi";
	VideoCapture box(path);
	if (box.isOpened())
	{
		cout << "Great success" << endl;
	}

	Mat img;
	int frame = 1, max;
	max = box.get(CAP_PROP_FRAME_COUNT);
	cout << box.get(CAP_PROP_FRAME_COUNT) << endl;
	while (frame < max)
	{

		box >> img;
		armorDetection(img);
		imshow("Image", img);
		frame++;

		waitKey(5);
	}

}
