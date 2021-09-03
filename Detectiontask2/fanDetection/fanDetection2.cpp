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
int hmax = 179, smax = 255, vmax = 255;//��ʼ���������


double getHistSimilarity(const Mat& I1, const Mat& I2)//�Դ��о�������ͨ�����طֲ�ֱ��ͼ������ԱȽ���ͼ�ĵ������ԣ���Χ��0��1
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

int matchTemplate(Mat imgOrigin)//ͨ��ģ��ƥ���жϳ���Ӧ��Ҷ�Ƿ��Ѿ�������
{
	string path2 = "Resources/fans(110.60)/hammer.jpg";
	Mat imgThammer = imread(path2);
	Mat imgTGray;
	cvtColor(imgThammer, imgTGray, COLOR_BGR2GRAY);
	double Similarity = getHistSimilarity(imgOrigin, imgTGray);//�ʹ���ģ��Ƚ�


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

float getDistance(Point pointO, Point pointA)//�����������
{
	float distance;
	distance = powf((pointO.x - pointA.x), 2) + powf((pointO.y - pointA.y), 2);
	distance = sqrtf(distance);
	return distance;
}

void fanDetection(Mat img)//Ԥ����ԭͼ��ͨ���������ȥ�룬Ȼ���ȡ��Ҷ��С��Ӿ��Σ�
						//����ȡ��͸��ͼ��ͨ��ģ��ƥ�䣬�����ԭͼ��������
{
	vector<Mat> imgChannels;
	split(img, imgChannels);
	//���Ŀ����ɫͼ��Ķ�ֵͼ

	Mat midImage = imgChannels.at(2) - imgChannels.at(0);//������ɫ
	threshold(midImage,mask, 100, 255, THRESH_BINARY);
	//Mat midImage = imgChannels.at(0) - imgChannels.at(2);

	Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));//�ߴ�Խ��������Խ��
	imshow("Mask", mask);
	dilate(mask, imgDil, kernel);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	int structElementSize = 1;

	Mat element = getStructuringElement(MORPH_RECT, Size(2 * structElementSize + 1, 2 * structElementSize + 1), Point(structElementSize, structElementSize));
	morphologyEx(imgDil, imgDil, MORPH_CLOSE, element);//�ջ����㣬����С�����ź���Ѱ����������װ�װ壩
	findContours(imgDil, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	imshow("imgDil", imgDil);
	structElementSize = 3;
	//imshow("ImageDil", imgDil);
	//cout<<"��������"<<contours.size()<<endl;//
	vector<RotatedRect> boundRect(contours.size());
	Point2f P[4];
	int n = 0;//�������  
	int k = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		//cout << "Area:" << area << endl;
		if (area > 2400)//����ͼ�����������������
		{

			boundRect[n] = minAreaRect(contours[i]);
			boundRect[n].points(P);
			//drawContours(img, contours, i, Scalar(255, 0, 255), 3);

			Point2f srcRect[4];
			Point2f dstRect[4];
			double width;
			double height;

			//������ȡ��ҶƬ�Ŀ��
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

			// Ӧ��͸�ӱ任�������ɹ������
			Mat transform = getPerspectiveTransform(srcRect, dstRect);
			Mat perspectMat;
			warpPerspective(imgDil, perspectMat, transform, Size(width, height));// ��ȡ��ҶͼƬ



			Mat testim = perspectMat(Rect(0, 0, width, height));
			Mat testimGray1, testimGray2;
			resize(testim, testim, Size(110, 60));

			double area2 = width * height;
			cout << "Area2:" << area2 << endl;
			if (area2 > 5000)//��һ�����˷���Ҷ����
			{


				imshow("ImageTestim", testim);
				matchTemplate(testim);//ģ��ƥ���ж��Ƿ�Ϊ��������Ҷ
				if (matchTemplate(testim) == hammer)
				{

					if (hierarchy[i][2] > 0)
					{
						drawContours(img, contours, i, Scalar(255, 0, 0), 3);
						RotatedRect rect_tmp = minAreaRect(contours[hierarchy[i][2]]);
						Point2f Pnt[4];
						rect_tmp.points(Pnt);
						//����ɸѡװ�װ�
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
						//ɸѡ
						if (height / width > maxHWRatio || area3 > maxArea || area3 < minArea)
						{
							continue;
						}
						Point centerP = rect_tmp.center;
						//�����
						circle(img, centerP, 1, Scalar(0, 0, 255), 1);
						//����װ��λ��
						for (int j = 0; j < 4; ++j)
						{
							line(img, Pnt[j], Pnt[(j + 1) % 4], Scalar(0, 255, 255), 2);
						}
					}
				}
			}
			n++;
			//cout << "��������" <<n << endl;
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
