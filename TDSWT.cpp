// TDSWT.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
/*
Copyright 2012 Andrew Perrault and Saurav Kumar.

This file is part of DetectText.

DetectText is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

DetectText is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with DetectText.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <cassert>
#include <fstream>
#include <exception>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "TextDetection.h"

using namespace std;
using namespace cv;
using namespace DetectText;

//图像转换
void convertToFloatImage(Mat& byteImage, Mat& floatImage)
{
	byteImage.convertTo(floatImage, CV_32FC1, 1.0 / 255.0, 0);
	//函数将源矩阵中的像素值转换为目标类型
	//convertTo(目标矩阵,需要的输出的矩阵类型,比例因子,将输入数组元素按比例缩放后添加的值)
}

//标准库的异常
class FeatureError : public std::exception 
{
	std::string message;
public:
	FeatureError(const std::string & msg, const std::string & file) {
		std::stringstream ss;

		ss << msg << " " << file;
		message = msg.c_str();
	}
	~FeatureError() throw () {
	}
};

Mat loadByteImage(const char * name) 
{
	Mat image = imread(name);

	if (image.empty()) {
		return Mat();
	}
	cvtColor(image, image, CV_BGR2RGB);
	//颜色空间转换（灰度空间转换）
	//cvtColor(输入图像,输出图像,颜色空间转换标识符)
	return image;
}

Mat loadFloatImage(const char * name) 
{
	Mat image = imread(name);

	if (image.empty()) {
		return Mat();
	}
	cvtColor(image, image, CV_BGR2RGB);
	Mat floatingImage(image.size(), CV_32FC3);
	image.convertTo(floatingImage, CV_32F, 1.0 / 255.0, 0);
	return floatingImage;
}

int mainTextDetection(int argc, char** argv) 
{
	//argv[1-3]: inputfilename, outputfilename, dark_on_light flag
	Mat byteQueryImage = loadByteImage(argv[1]);
	if (byteQueryImage.empty()) {
		cerr << "couldn't load query image" << endl;
		return -1;
	}

	// Detect text in the image
	Mat output = textDetection(byteQueryImage, atoi(argv[3]));
	imwrite(argv[2], output);
	namedWindow("Final image", CV_WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Final image", output);
///////////////////////////////convert final image to binary image///////////////////////////
	Mat outBin(output.size(), CV_8UC1);
	//cvtColor(output, outBin, CV_RGB2GRAY);
	//binarization
	int i, j, pos, pos1;
	int w, h, wb, wb1;
	unsigned char *pData, *pData1;
	w = outBin.cols; h = outBin.rows;
	wb = outBin.step[0]; wb1 = output.step[0];
	pData = (unsigned char*)(outBin.data);
	pData1 = (unsigned char*)(output.data);
#if 0//output1, invert color to black foreground
	for (pos = 0, pos1 = 0, i = 0; i < h; i ++, pos += wb, pos1 += wb1)
	{
		for (j = 0; j < w; j++)
		{
			if (pData1[pos1 + j] >= 128)
				pData[pos + j] = 0;//foreground
			else
				pData[pos + j] = 255;
		}
	}
#else//output2
	for (pos = 0, pos1 = 0, i = 0; i < h; i ++, pos += wb, pos1 += wb1)
	{
		for (j = 0; j < w; j++)
		{
			if (pData1[pos1 + j] < 255)
				pData[pos + j] = 0;//foreground
			else
				pData[pos + j] = 255;
		}
	}
#endif
	imwrite("outbin.png", outBin);
///////////////////////////////////////////////////////////////////////////////////////////

	cvWaitKey();
	return 0;
}

//int main(int argc, char** argv) {
//	if ((argc != 4)) {
//		cerr << "usage: " << argv[0] << " imagefile resultImage darkText" << endl;
//		return -1;
//	}
//	return mainTextDetection(argc, argv);
//}

int _tmain(int argc, _TCHAR* argv[])
{
	if ((argc != 4)) 
	{
		//cerr << "usage: " << argv[0] << " imagefile resultImage darkText" << endl;
		return -1;
	}
	return mainTextDetection(argc, argv);
}

