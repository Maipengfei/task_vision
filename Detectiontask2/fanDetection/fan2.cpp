#include <opencv2/imgcodecs.hpp> 
#include <opencv2/highgui.hpp> 
#include <opencv2/imgproc.hpp> 
#include <iostream>

using namespace std;
using namespace cv;

/// ///////////Fan /////////////////
enum State { hammer, sword, neither };
Mat imgCanny, imgDil, imgCannyDil;
Mat imgHSV, mask, imgT1, imgT2, imgT1Gray, imgT2Gray;
int hmin = 0, smin = 151, vmin = 130;
int hmax = 179, smax = 255, vmax = 255;//初始化各项参数


double getHistSimilarity(const Mat& I1, const Mat& I2)//仍待研究！！！通过像素分布直方图的相关性比较两图的的相似性，范围从0到1
{
	int histSize = 256;
	float range[] = { 0,256 };
	const float* histRange = { range };
	bool uniform = true;
	bool accumulate = false;

	Mat hist1, hist2;

	calcHist(&I1, 1, 0, Mat(), hist1, 1, &histSize, &histRange, uniform, accumulate);
	normalize(hist1, hist1, 0, 1, NORM_MINMAX, -1, Mat());

	calcHist(&I2, 1, 0, Mat(), hist2, 1, &histSize, &histRange, uniform, accumulate);
	normalize(hist2, hist2, 0, 1, NORM_MINMAX, -1, Mat());

	return compareHist(hist1, hist2, HISTCMP_CORREL);
}

int matchTemplate(Mat imgOrigin)//通过模板匹配判断出对应扇叶是否已经被击打
{
	string path2 = "Resources/fans(110.60)/hammer.jpg";
	Mat imgThammer = imread(path2);
	Mat imgTGray;
	cvtColor(imgThammer, imgTGray, COLOR_BGR2GRAY);
	double Similarity = getHistSimilarity(imgOrigin, imgTGray);//和锤子模板比较


	cout << "Similarity:" << Similarity << endl;
	if (Similarity > 0.94)
	{
		//cout << "It is hammer";
		return hammer;
	}
	else if (Similarity > 0.80)
	{
		//cout << "It is sword";
		return sword;
	}
	imshow("Template", imgTGray);
}

float getDistance(Point pointO, Point pointA)//计算两点距离
{
	float distance;
	distance = powf((pointO.x - pointA.x), 2) + powf((pointO.y - pointA.y), 2);
	distance = sqrtf(distance);
	return distance;
}

void fanDetection(Mat img)//预处理原图，通过两次面积去噪，然后获取扇叶最小外接矩形，
						//并获取其透视图，通过模板匹配，最后在原图标出打击点
{
	vector<Mat> imgChannels;
	split(img, imgChannels);
	//获得目标颜色图像的二值图

	Mat midImage = imgChannels.at(2) - imgChannels.at(0);//保留红色
	threshold(midImage,mask, 100, 255, THRESH_BINARY);
	//Mat midImage = imgChannels.at(0) - imgChannels.at(2);

	Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));//尺寸越大，则膨胀越多
	imshow("Mask", mask);
	dilate(mask, imgDil, kernel);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	int structElementSize = 1;

	Mat element = getStructuringElement(MORPH_RECT, Size(2 * structElementSize + 1, 2 * structElementSize + 1), Point(structElementSize, structElementSize));
	morphologyEx(imgDil, imgDil, MORPH_CLOSE, element);//闭环运算，避免小洞干扰后续寻找子轮廓（装甲板）
	findContours(imgDil, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	imshow("imgDil", imgDil);
	structElementSize = 3;
	//imshow("ImageDil", imgDil);
	//cout<<"轮廓数："<<contours.size()<<endl;//
	vector<RotatedRect> boundRect(contours.size());
	Point2f P[4];
	int n = 0;//计算灯柱  
	int k = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		//cout << "Area:" << area << endl;
		if (area > 2400)//根据图像面积过滤噪音轮廓
		{

			boundRect[n] = minAreaRect(contours[i]);
			boundRect[n].points(P);
			//drawContours(img, contours, i, Scalar(255, 0, 255), 3);

			Point2f srcRect[4];
			Point2f dstRect[4];
			double width;
			double height;

			//矫正提取的叶片的宽高
			width = getDistance(P[0], P[1]);
			height = getDistance(P[1], P[2]);
			if (width > height)
			{
				srcRect[0] = P[0];
				srcRect[1] = P[1];
				srcRect[2] = P[2];
				srcRect[3] = P[3];
			}
			else
			{
				swap(width, height);
				srcRect[0] = P[1];
				srcRect[1] = P[2];
				srcRect[2] = P[3];
				srcRect[3] = P[0];
			}
			dstRect[0] = Point2f(0, 0);
			dstRect[1] = Point2f(width, 0);
			dstRect[2] = Point2f(width, height);
			dstRect[3] = Point2f(0, height);

			// 应用透视变换，矫正成规则矩形
			Mat transform = getPerspectiveTransform(srcRect, dstRect);
			Mat perspectMat;
			warpPerspective(imgDil, perspectMat, transform, Size(width, height));// 提取扇叶图片



			Mat testim = perspectMat(Rect(0, 0, width, height));
			Mat testimGray1, testimGray2;
			resize(testim, testim, Size(110, 60));

			double area2 = width * height;
			cout << "Area2:" << area2 << endl;
			if (area2 > 5000)//进一步过滤非扇叶轮廓
			{


				imshow("ImageTestim", testim);
				matchTemplate(testim);//模板匹配判断是否为击打后的扇叶
				if (matchTemplate(testim) == hammer)
				{

					if (hierarchy[i][2] > 0)
					{
						drawContours(img, contours, i, Scalar(255, 0, 0), 3);
						RotatedRect rect_tmp = minAreaRect(contours[hierarchy[i][2]]);
						Point2f Pnt[4];
						rect_tmp.points(Pnt);
						//用于筛选装甲板
						const float maxHWRatio = 0.7;
						const float maxArea = 1400;
						const float minArea = 500;

						float width = rect_tmp.size.width;
						float height = rect_tmp.size.height;
						if (height > width)
							swap(height, width);
						float area3 = width * height;
						cout << "Area3:" << area3 << endl;
						cout << "ratio:" << (height / width) << endl;
						//筛选
						if (height / width > maxHWRatio || area3 > maxArea || area3 < minArea)
						{
							continue;
						}
						Point centerP = rect_tmp.center;
						//打击点
						circle(img, centerP, 1, Scalar(0, 0, 255), 1);
						//画出装甲位置
						for (int j = 0; j < 4; ++j)
						{
							line(img, Pnt[j], Pnt[(j + 1) % 4], Scalar(0, 255, 255), 2);
						}
					}
				}
			}
			n++;
			//cout << "轮廓数：" <<n << endl;
		}
	}

	imshow("Image", img);
	//imshow("ImageDil", imgDil);
	waitKey(10);
}

/********     Basic Functions    ***********/
void main()
{
	namedWindow("Image", WINDOW_NORMAL);
	namedWindow("imgDil", WINDOW_NORMAL);
	namedWindow("Mask", WINDOW_NORMAL);
	string path = "Resources/RM2021fan.avi";
	VideoCapture fanCap(path);

	Mat img;
	int frame = 4, max;
	max = fanCap.get(CAP_PROP_FRAME_COUNT);
	cout << "frame" << max << endl;
	while (1)
	{
		fanCap >> img;
		fanDetection(img);
		frame++;
	}

}
