#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int app_main()
{
	string path = "Resources/test.png";
	cout << path << endl;
	Mat img = imread(path);
	imshow("Image", img);
	waitKey(0); //��ʾͼƬ����һ������
	return 0;
}