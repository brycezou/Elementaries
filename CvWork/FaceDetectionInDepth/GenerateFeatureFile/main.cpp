#include "obtainForegroundMask.h"
#include "FourierTransform.h"
#include "LocateHeadRegion.h"
#include "registerColor2Depth.h"
#include "FindContours.h"
#include "GetHeadRegionRect.h"
#include <fstream>
#include "svm.h"

using namespace std;


//四个高级分块的遮挡情况, 1为遮挡, 0为不遮挡
int EYES_OCCLUDED = 1;
int NOSE_OCCLUDED = 0;
int MOUTH_OCCLUDED = 1;
int CHIN_OCCLUDED = 0; 

//在深度图像depthIpl和配准后的彩色图像regColorIpl上
//标记出人脸区域headRect
//说明：直接在原图上标记
void showHeadRectResult(IplImage *depthIpl, IplImage *regColorIpl, CvRect &headRect)
{
	cvRectangle(depthIpl, cvPoint(2*headRect.x, 2*headRect.y), cvPoint(2*(headRect.x+headRect.width), 2*(headRect.y+headRect.height)), CV_RGB(0,0,128), 2);
	cvShowImage("深度人头定位结果", depthIpl);
	cvRectangle(regColorIpl, cvPoint(2*headRect.x, 2*headRect.y), cvPoint(2*(headRect.x+headRect.width), 2*(headRect.y+headRect.height)), CV_RGB(255,0,0), 2);
	cvShowImage("彩色人头定位结果", regColorIpl);
}

//缩放输入图像src至新的尺寸
//缩放后的宽度为newWidth
//缩放后的高度为newHeight
//说明：会返回新的图像
IplImage* resizeInputImage(IplImage *src, int newWidth, int newHeight)
{
	IplImage *newSizeImg = cvCreateImage(cvSize(newWidth, newHeight), src->depth, src->nChannels);
	cvResize(src, newSizeImg, CV_INTER_LINEAR);
	return newSizeImg;
}

//在48x56的人脸区域上预定义的区域
CvRect EYES_RECT	= cvRect(6, 16, 36, 16);		//眼睛区域
CvRect NOSE_RECT	= cvRect(6, 24, 36, 16);		//鼻子区域
CvRect MOUTH_RECT	= cvRect(6, 32, 36, 16);	//嘴巴区域
CvRect CHIN_RECT = cvRect(6, 48, 36, 8);			//下巴区域

//高级人脸分块的特征向量
//彩色图像的彩色直方图, 彩色图像的灰度直方图
//彩色图像的Haar特征, 深度图像的Haar特征
int f_vc[4172];	

//保存四个高级人脸分块的特征
ofstream ofile_eyes("eyes.txt", ios::app);
ofstream ofile_nose("nose.txt", ios::app);
ofstream ofile_mouth("mouth.txt", ios::app);
ofstream ofile_chin("chin.txt", ios::app);

//得到图像color_img的彩色直方图(每个通道取16级灰度), 保存在f_vc数组的[0:4095]位
//和灰度直方图(64级灰度), 保存在f_vc数组的[4096:4159]位
void getColorAndGrayHistogram(IplImage *color_img)
{
	memset(f_vc, 0, sizeof(int)*4172);
	for (int i = 0; i < color_img->height; i++)
	{
		for (int j = 0; j < color_img->width; j++)
		{
				int rr = ((uchar*)(color_img->imageData+i*color_img->widthStep))[j*color_img->nChannels+0]; 
				int gg = ((uchar*)(color_img->imageData+i*color_img->widthStep))[j*color_img->nChannels+1]; 
				int bb = ((uchar*)(color_img->imageData+i*color_img->widthStep))[j*color_img->nChannels+2]; 
				f_vc[rr/16*256+gg/16*16+bb/16]++;							//统计彩色直方图 [0:4095]
				f_vc[((int)(0.30*rr+0.59*gg+0.11*bb))/4+4096]++;	//统计灰度直方图 [4096:4159]
		}
	}
}

//得到图像color_roi的特征并写到文本文件里
void getFeatureVectors4Color(IplImage *color_roi)
{
	//眼睛区域
	cvSetImageROI(color_roi, EYES_RECT);
	//cvSaveImage("eyes.jpg", color_roi);
	getColorAndGrayHistogram(color_roi);
	ofile_eyes<<EYES_OCCLUDED<<" ";
	for (int i = 0; i < 4172; i++)
		ofile_eyes<<i+1<<":"<<f_vc[i]<<" ";
	ofile_eyes<<endl;
	cvResetImageROI(color_roi);

	//鼻子区域
	cvSetImageROI(color_roi, NOSE_RECT);
	//cvSaveImage("nose.jpg", color_roi);
	getColorAndGrayHistogram(color_roi);
	ofile_nose<<NOSE_OCCLUDED<<" ";
	for (int i = 0; i < 4172; i++)
		ofile_nose<<i+1<<":"<<f_vc[i]<<" ";
	ofile_nose<<endl;
	cvResetImageROI(color_roi);

	//嘴巴区域
	cvSetImageROI(color_roi, MOUTH_RECT);
	//cvSaveImage("mouth.jpg", color_roi);
	getColorAndGrayHistogram(color_roi);
	ofile_mouth<<MOUTH_OCCLUDED<<" ";
	for (int i = 0; i < 4172; i++)
		ofile_mouth<<i+1<<":"<<f_vc[i]<<" ";
	ofile_mouth<<endl;
	cvResetImageROI(color_roi);

	//下巴区域
	cvSetImageROI(color_roi, CHIN_RECT);
	//cvSaveImage("chin.jpg", color_roi);
	getColorAndGrayHistogram(color_roi);
	ofile_chin<<CHIN_OCCLUDED<<" ";
	for (int i = 0; i < 4172; i++)
		ofile_chin<<i+1<<":"<<f_vc[i]<<" ";
	ofile_chin<<endl;
	cvResetImageROI(color_roi);
}

void generateFeatureFiles(IplImage *depthIpl, IplImage *regColorIpl, CvRect &headRect)
{
	//获得人脸感兴趣区域
	CvRect rect_roi = cvRect(2*headRect.x, 2*headRect.y, 2*headRect.width, 2*headRect.height);

	//将深度人脸图像缩放到48*56像素
	cvSetImageROI(depthIpl, rect_roi);
	IplImage *depth_roi = resizeInputImage(depthIpl, 48, 56);
	cvResetImageROI(depthIpl);

	//将彩色人脸图像缩放到48*56像素
	cvSetImageROI(regColorIpl, rect_roi);
	IplImage *color_roi = resizeInputImage(regColorIpl, 48, 56);
	cvResetImageROI(regColorIpl);

	//cvShowImage("depth_roi", depth_roi);
	//cvShowImage("color_roi", color_roi);

	char ch = cvWaitKey(0);
	switch(ch)
	{
	case 's':
		//计算彩色人脸和深度人脸的特征
		getFeatureVectors4Color(color_roi);
		cout<<"saved"<<endl;
		break;
	default:
		cout<<"not saved"<<endl;
		break;
	}

	cvReleaseImage(&color_roi);
	cvReleaseImage(&depth_roi);
}


int main(int argc, char **argv)
{
	//以深度视频和彩色视频为参数, 获取特征文件
	if(argc != 3) {
		cout<<"only two argument \"[depth video] [color video]\" required"<<endl;	
		system("pause");	return -1;
	}
	//读取深度视频
	CvCapture *capDepth = cvCreateFileCapture(argv[1]);
	if (NULL == capDepth) {
		cout<<"depth video is invalid"<<endl;
		system("pause");	return -1;
	}
	//读取彩色视频
	CvCapture *capColor = cvCreateFileCapture(argv[2]);
	if (NULL == capColor) {
		cout<<"color video is invalid"<<endl;
		system("pause");	return -1;
	}
	//输入4个高级分块的遮挡情况
	while(true) {
		cout<<"Eyes? Please input 1 for occluded or 0 for not."<<endl;
		cin>>EYES_OCCLUDED;
		cout<<"Nose? Please input 1 for occluded or 0 for not."<<endl;
		cin>>NOSE_OCCLUDED;
		cout<<"Mouth? Please input 1 for occluded or 0 for not."<<endl;
		cin>>MOUTH_OCCLUDED;
		cout<<"Chin? Please input 1 for occluded or 0 for not."<<endl;
		cin>>CHIN_OCCLUDED;
		cout<<"Please confirm: ";
		cout<<EYES_OCCLUDED<<", "<<NOSE_OCCLUDED<<", "<<MOUTH_OCCLUDED<<", "<<CHIN_OCCLUDED;
		cout<<" (y/~y?)  ";
		char ch;
		bool ifgon = false;
		cin>>ch;
		switch(ch)
		{
		case 'y':
			ifgon = true;
			break;
		default:
			ifgon = false;
			break;
		}
		if(ifgon)
			break;
		else
			cout<<endl<<endl;
	}
	//注意:保存的深度视频帧为3通道, 而算法中使用的是单通道
	IplImage *frame_depth_3c = cvQueryFrame(capDepth);
	IplImage *depthIpl = cvCreateImage(cvSize(640,480), 8, 1);
	IplImage *colorIpl = cvQueryFrame(capColor);
	IplImage *depth2show = NULL;
	IplImage *color2show = NULL;
	CvRect headRect;
	while((frame_depth_3c != NULL) && (colorIpl != NULL))
	{
		//将三通道的深度视频帧转换成单通道的灰度图像
		cvCvtColor(frame_depth_3c, depthIpl, CV_BGR2GRAY);
		depth2show = cvCloneImage(depthIpl);
		IplImage *regColorIpl = GetHeadRegionRect(depthIpl, colorIpl, headRect, false);
		if (regColorIpl != NULL)
		{
			color2show = cvCloneImage(regColorIpl);
			showHeadRectResult(depth2show, color2show, headRect);
			generateFeatureFiles(depthIpl, regColorIpl, headRect);
			cvReleaseImage(&regColorIpl);
			cvReleaseImage(&color2show);
		}
		cvReleaseImage(&depth2show);
		frame_depth_3c = cvQueryFrame(capDepth);
		colorIpl = cvQueryFrame(capColor);
	}
	cout<<"finished!"<<endl;
	cvReleaseImage(&colorIpl);
	cvReleaseImage(&depthIpl);
	cvReleaseImage(&frame_depth_3c);

	return 0;
}
