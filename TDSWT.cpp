// TDSWT.cpp : �������̨Ӧ�ó������ڵ㡣
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

//ͼ��ת��
void convertToFloatImage(Mat& byteImage, Mat& floatImage)
{
	byteImage.convertTo(floatImage, CV_32FC1, 1.0 / 255.0, 0);
	//������Դ�����е�����ֵת��ΪĿ������
	//convertTo(Ŀ�����,��Ҫ������ľ�������,��������,����������Ԫ�ذ��������ź���ӵ�ֵ)
}

//��׼����쳣
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
	//��ɫ�ռ�ת�����Ҷȿռ�ת����
	//cvtColor(����ͼ��,���ͼ��,��ɫ�ռ�ת����ʶ��)
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

