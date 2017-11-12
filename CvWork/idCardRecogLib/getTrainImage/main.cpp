#include <iostream>
#include "highgui.h"
#include "cv.h"
#include "BlobResult.h"
#include "blob.h"
#include <list>

using namespace std;
using namespace cv;

#define FIXED_WIDTH	600


IplImage* ResizeInputImage(IplImage *src)
{
	//构造缩小后的目标图象
	IplImage *newImg = cvCreateImage(cvSize(FIXED_WIDTH, src->height*FIXED_WIDTH/1.0/src->width), src->depth, src->nChannels);
	cvResize(src, newImg, CV_INTER_LINEAR);
	return newImg;
}

//计算输入灰度图像srcGray的自适应化二值图像
//将结果反映到输出图像dstBinMat中
void getBinaryImageAdaptive(IplImage* srcGray, Mat &dstBinMat)
{
	Mat srcGrayMat(srcGray);
	adaptiveThreshold(srcGrayMat, dstBinMat, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 9, 8);
}

IplImage* getNumberBlobs(IplImage *srcBinary, list<CvRect> &rctList)
{
	rctList.clear();
	CBlobResult blobs;
	CBlob *currentBlob;
	IplImage *blobRGB = cvCreateImage(cvGetSize(srcBinary), IPL_DEPTH_8U, 3);
	blobs = CBlobResult( srcBinary, NULL, 0 );												//寻找二值图中的blobs
	blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, 10 );
	cvMerge(  srcBinary,  srcBinary,  srcBinary, NULL, blobRGB );		
	for (int j = 0; j < blobs.GetNumBlobs(); j++ )
	{
		currentBlob = blobs.GetBlob(j);
		CvRect rct = currentBlob->GetBoundingBox();										//得到当前连通块的外包矩形
		int wi = rct.width;
		int he = rct.height;
		int Height = srcBinary->height;
		int Width = srcBinary->width;
		if (	/*wi*1.0/he > 0.2 && wi*1.0/he < 5*/											//宽高比滤波
			/*&&*/ wi*he > Width*Height/1.0/10000									//字符面积滤波
			)			
		{
			currentBlob->FillBlob( blobRGB, CV_RGB(0, 0, 255));
			cvRectangle(blobRGB, cvPoint(rct.x, rct.y), cvPoint(rct.x+rct.width, rct.y+rct.height), cvScalar(0, 255, 0), 1);
			rctList.push_back(rct);
		}
	}
	return blobRGB;
}


int main(int argc, char **argv)
{
	IplImage *srcGray = cvLoadImage(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	Mat binaryMat;
	getBinaryImageAdaptive(srcGray, binaryMat);
	cvReleaseImage(&srcGray);
	IplImage binaryImg = binaryMat;	

	char strName[500];
	memset(strName, '\0', sizeof(char)*500);
	list<CvRect> rctList;
	IplImage *blobRGB = getNumberBlobs(&binaryImg, rctList);
	int i = 0;
	for (list<CvRect>::iterator itor = rctList.begin(); itor != rctList.end(); itor++, i++)
	{
		cvSetImageROI(&binaryImg, *itor);
		IplImage *tmp = cvCreateImage(cvSize(32, 32), IPL_DEPTH_8U, 1);
		cvResize(&binaryImg, tmp, CV_INTER_LINEAR);
		sprintf(strName, "%s_%04d.jpg", argv[1], i);
		cvSaveImage(strName, tmp);
		cvReleaseImage(&tmp);
		cvResetImageROI(&binaryImg);
	}
	cvShowImage("blobRGB", blobRGB);
	cvReleaseImage(&blobRGB);
	binaryMat.release();
	rctList.clear();

	cvWaitKey(0);
	return 0;
}