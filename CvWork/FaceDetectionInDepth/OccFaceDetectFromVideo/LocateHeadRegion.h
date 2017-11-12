#include <opencv2/opencv.hpp>
#include "TwoClassOTSU.h"
#include <fstream>
#include <iostream>
using namespace std;


#define BOTTOM_FILTER_SIZE	20 
#define CORRECT_DRIFT_LENGTH	2
#define OUT_FILE_NAME ("out.txt")

//自定义的目标函数的参数
typedef struct ParaArea
{
	int s1;
	int s2;
	int s3;
	int s4;
}ParaArea;

//在中值滤波后的mask图像maskIpl中，
//使用双重投影法定位人头区域
CvRect LocateHeadRegionRoughly(const IplImage *maskIpl, bool bShowMidImg) 
{
	int nVerticalProjImageWidth = maskIpl->width;		//垂直投影图像的宽度
	int  *verticalProjHist = new int[nVerticalProjImageWidth];	//垂直投影直方图
	memset(verticalProjHist, 0, nVerticalProjImageWidth*sizeof(int));
	int nMaxVal = -1;		//垂直投影直方图的最大值
	for (int j = 0; j < maskIpl->width; j++) 
	{
		for (int i = 0; i < maskIpl->height; i++) {
			int pixVal = ((uchar *)(maskIpl->imageData+i*maskIpl->widthStep))[j];
			verticalProjHist[j] += pixVal > 128 ? 1 : 0;
		}
		if(verticalProjHist[j] > nMaxVal)	
			nMaxVal = verticalProjHist[j];
	}
	//垂直投影图像的高度
	int nVerticalProjImageHeight = nMaxVal;

	//垂直投影图像
	IplImage *verticalProjIplImage = cvCreateImage(cvSize(nVerticalProjImageWidth, nVerticalProjImageHeight), 8, 1);
	cvZero(verticalProjIplImage);
	for (int i = 0; i < nVerticalProjImageWidth; i++) {
		//cvLine(verticalProjIplImage, cvPoint(i, 0), cvPoint(i, verticalProjHist[i]-1), CV_RGB(0,0,255), 1);
		cvLine(verticalProjIplImage, cvPoint(i, nVerticalProjImageHeight-1), cvPoint(i, nVerticalProjImageHeight-verticalProjHist[i]), CV_RGB(0,0,255), 1);
	}
	if(bShowMidImg) {
		cvShowImage("垂直投影图像", verticalProjIplImage);
	}
	delete []verticalProjHist;

	int nHorizentalProjImageHeight = nMaxVal;	//水平投影图像的高度
	int  *horizontalProjHist = new int[nHorizentalProjImageHeight];		//水平投影直方图
	memset(horizontalProjHist, 0, nHorizentalProjImageHeight*sizeof(int));
	nMaxVal = -1;		//水平投影直方图的最大值
	for (int i = 0; i < verticalProjIplImage->height; i++) 
	{
		for (int j = 0; j < verticalProjIplImage->width; j++) {
			int pixVal = ((uchar *)(verticalProjIplImage->imageData+i*verticalProjIplImage->widthStep))[j];
			horizontalProjHist[i] += pixVal > 128 ? 1 : 0;
		}
		if(horizontalProjHist[i] > nMaxVal)
			nMaxVal = horizontalProjHist[i];
	}
	//水平投影图像的宽度
	int nHorizentalProjImageWidth = nMaxVal;

	//滤除底部可能存在的干扰像素
	nHorizentalProjImageHeight = nHorizentalProjImageHeight-BOTTOM_FILTER_SIZE;
	if(nHorizentalProjImageHeight < BOTTOM_FILTER_SIZE)
	{
		delete []horizontalProjHist;
		cvReleaseImage(&verticalProjIplImage);
		return cvRect(-1, -1, -1, -1);
	}

	//水平投影图像
	IplImage *horizontalProjIplImage = cvCreateImage(cvSize(nHorizentalProjImageWidth, nHorizentalProjImageHeight), 8, 1);
	cvZero(horizontalProjIplImage);
	for (int i = 0; i < nHorizentalProjImageHeight; i++) {
		cvLine(horizontalProjIplImage, cvPoint(0, i), cvPoint(horizontalProjHist[i]-1, i), CV_RGB(0,0,255), 1);
	}
	if (bShowMidImg) {
		cvShowImage("水平投影图像", horizontalProjIplImage);
	}
	
	//计算积分向量
	int *projIntegral = new int[nHorizentalProjImageHeight];
	projIntegral[0] = horizontalProjHist[0];
	for (int i = 1; i < nHorizentalProjImageHeight; i++) {
		projIntegral[i] = projIntegral[i-1]+horizontalProjHist[i];
	}

	//自定义目标函数在每一个点需要的参数
	ParaArea *paraArea = new ParaArea[nHorizentalProjImageHeight];
	//自定义目标函数在每一点的取值
	double *ratio = new double[nHorizentalProjImageHeight];
	double maxRatio = 0;
	int maxIndex = 0;
	//保存目标函数的值, 用于分析函数值的变化情况
	//ofstream ofile(OUT_FILE_NAME);

	for (int i = 1; i < nHorizentalProjImageHeight-1; i++)
	{
		paraArea[i].s1 = projIntegral[i];
		paraArea[i].s2 = (i+1)*horizontalProjHist[i]-paraArea[i].s1;
		paraArea[i].s3 = projIntegral[nHorizentalProjImageHeight-1]-paraArea[i].s1;
		paraArea[i].s4 = (nHorizentalProjImageHeight-i)*horizontalProjHist[nHorizentalProjImageHeight-1]-paraArea[i].s3;
		ratio[i] = (paraArea[i].s1/1.0/paraArea[i].s2)*(paraArea[i].s1/1.0/paraArea[i].s2)*(paraArea[i].s3/1.0/paraArea[i].s4);
		//ofile<<ratio[i]<<endl;		//输出目标函数值
	}
	delete []paraArea;
	delete []projIntegral;

	int thresh = TwoClassOTSU(ratio, BOTTOM_FILTER_SIZE/2, nHorizentalProjImageHeight-BOTTOM_FILTER_SIZE/2, ADV_OPT);
	for (int i = 1; i < nHorizentalProjImageHeight-1; i++) {
		if (i >= thresh)	break;
		//增加i > thresh/2约束于2015-11-12
		if (ratio[i] > maxRatio && i > thresh/2) {
			maxRatio = ratio[i];
			maxIndex = i;
		}
	}
	
	int nFaceWidth = horizontalProjHist[maxIndex];
	int nFaceHeight = maxIndex;
	//ofile<<nFaceHeight<<endl;
	//ofile.close();
	delete []ratio;
	delete []horizontalProjHist;

	if(bShowMidImg) {
		cvLine(horizontalProjIplImage, cvPoint(0, nFaceHeight), cvPoint(nHorizentalProjImageWidth-1, nFaceHeight), CV_RGB(0,0,128), 1);
		cvShowImage("确定高度宽度_在水平投影图像中", horizontalProjIplImage);
	}
	cvReleaseImage(&horizontalProjIplImage);
	
	//以此反向寻找人头区域的左边界, 右边界, 顶边界和底边界
	int leftBorder = 0, rightBorder, topBorder, counter = 0, maxCounter = 0;
	for (int j = 1; j < verticalProjIplImage->width; j++) {
		//在底边界maxIndex上寻找左边界
		int preVal = ((uchar *)(verticalProjIplImage->imageData+maxIndex*verticalProjIplImage->widthStep))[j-1];
		int curVal = ((uchar *)(verticalProjIplImage->imageData+maxIndex*verticalProjIplImage->widthStep))[j];
		if(preVal < 128 && curVal >128) {
			counter = 1;
		} else if(preVal > 128 && curVal >128) {
			counter++;
		} else if(preVal > 128 && curVal < 128) {
			if(counter > maxCounter) {
				maxCounter = counter;
				rightBorder = j;	
			}
		}
	}

	nFaceWidth = maxCounter*11/10;
	rightBorder = rightBorder + maxCounter/20;
	leftBorder = rightBorder - maxCounter*21/20;

	if (bShowMidImg) {
		cvLine(verticalProjIplImage, cvPoint(leftBorder, 0), cvPoint(leftBorder, nVerticalProjImageHeight-1), CV_RGB(0,0,128), 1);
		cvLine(verticalProjIplImage, cvPoint(rightBorder, 0), cvPoint(rightBorder, nVerticalProjImageHeight-1), CV_RGB(0,0,128), 1);
		cvLine(verticalProjIplImage, cvPoint(0, nFaceHeight), cvPoint(nVerticalProjImageWidth-1, nFaceHeight), CV_RGB(0,0,128), 1);
		cvShowImage("确定左右边界_在垂直投影图像中", verticalProjIplImage);
	}

	topBorder = maskIpl->height-(nHorizentalProjImageHeight+BOTTOM_FILTER_SIZE);
	CvRect rct = cvRect(leftBorder-CORRECT_DRIFT_LENGTH, topBorder, nFaceWidth-1, nFaceHeight);

	cvReleaseImage(&verticalProjIplImage);
	return rct;
}