#include <iostream>
#include <fstream>
#include "highgui.h"
#include "cv.h"
#include "ittFann.h"
#include <Windows.h>

using namespace std;


int main(int argc, char **argv)
{
	ittFann ittfann2;
	ittfann2.fannCreateFromFile("model.txt", false);
	fann_type input[1024];

	IplImage *srcGray = NULL;
	for (int k = 0; k < argc-1; k++)
	{
		srcGray = cvLoadImage(argv[k+1], CV_LOAD_IMAGE_GRAYSCALE);
		cvThreshold(srcGray, srcGray, 25, 255, CV_THRESH_OTSU);
		for (int i = 0; i < srcGray->height; i++)
		{
			for (int j = 0; j < srcGray->width; j++)
			{
				char tmp = ((uchar *)(srcGray->imageData+i*srcGray->widthStep))[j];
				//cout<<-(int)(tmp)<<"  ";
				input[i*32+j] = -(int)(tmp);
			}
		}
	}
	cvReleaseImage(&srcGray);

	fann_type *output = ittfann2.fannPredict(input, 1024);
	int maxv = -10, maxi = -1;
	for (unsigned int i = 0; i < ittfann2.m_ann->num_output; i++)
	{
		//cout<<output[i]<<endl;
		if (output[i] > maxv)
		{
			maxv = output[i];
			maxi = i;
		}
	}
	cout<<maxi<<endl;
	ittfann2.fannRelease();

	//将32x32的图像转换成FANN要求的格式
	//IplImage *srcGray = NULL;
	//ofstream outfile("9.txt");
	//outfile<<argc-1<<'\t'<<32*32<<'\t'<<10<<endl;
	//for (int k = 0; k < argc-1; k++)
	//{
	//	srcGray = cvLoadImage(argv[k+1], CV_LOAD_IMAGE_GRAYSCALE);
	//	cvThreshold(srcGray, srcGray, 25, 255, CV_THRESH_OTSU);
	//	for (int i = 0; i < srcGray->height; i++)
	//	{
	//		for (int j = 0; j < srcGray->width; j++)
	//		{
	//			char tmp = ((uchar *)(srcGray->imageData+i*srcGray->widthStep))[j];
	//			outfile<<-(int)(tmp)<<"  ";
	//		}
	//	}
	//	outfile<<endl<<"-1 -1 -1 -1 -1 -1 -1 -1 -1 1"<<endl;
	//}
	//outfile.close();
	while (true)
	{
		Sleep(1000);
	}
	return 0;
}