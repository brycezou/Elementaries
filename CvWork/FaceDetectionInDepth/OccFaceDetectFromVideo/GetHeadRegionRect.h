#include <opencv2/opencv.hpp>


//是否显示前景背景分离的中间结果
bool B_FORGROUND = false;
//是否显示人头定位的中间结果
bool B_LOCATE_HEAD = false;


float GF_SCALE = 0.5;

/*
double gWarp[] = {	0.928764,			-0.0790137,			-0.585636,		
								-0.0436254,			0.947218,			-0.623276,		
								-0.000121214,		-0.000322128,		1					};
*/

double gWarp[] = {	1.36788,				0.138139,			-112.907,		
								0.0304907,			1.43976,				-76.9039,	
								9.79889e-005,		0.000453478,		1				};


//根据参数fScale对输入图像srcImg进行缩放
//返回值为缩放后的图像
IplImage* resizeImage(IplImage *srcImg, float fScale)
{
	int w = srcImg->width*fScale;
	int h = srcImg->height*fScale;
	IplImage *dstImg = cvCreateImage(cvSize(w, h), srcImg->depth, srcImg->nChannels);
	cvResize(srcImg, dstImg, CV_INTER_LINEAR);  
	return dstImg;
}

//在深度图像depthIpl中定位人头区域，并将结果保存在参数headRect中
//对彩色图像colorIpl进行配准
//返回值为配准后的彩色图像
//参数bShowMidImg用于控制是否显示中间结果
IplImage* GetHeadRegionRect(IplImage *depthIpl, IplImage *colorIpl, CvRect &headRect, bool bShowMidImg)
{
	//从深度图像中得到前景目标掩膜图像
	IplImage *maskIpl = getForegroundMask(depthIpl, B_FORGROUND);
	if(maskIpl==NULL)
		return NULL;

	//将彩色图像配准到深度图像
	CvMat homography = cvMat( 3, 3, CV_64FC1, gWarp);
	IplImage *regColorIpl = RegisterColor2Depth(colorIpl, &homography);
	////将彩色图像和灰度图像绘制在同一个坐标系下
	////仅用于显示中间结果，无实质性作用，可删除
	//for (int i = 0; i < maskIpl->height; i++) {
	//	for(int j = 0; j < maskIpl->width; j++) {
	//		int pixVal = ((uchar *)(maskIpl->imageData+i*maskIpl->widthStep))[j];
	//		if (pixVal < 128) {
	//			//在未配准图像上标记
	//			((uchar*)(colorIpl->imageData+i*colorIpl->widthStep))[j*colorIpl->nChannels+0] = 0; 
	//			((uchar*)(colorIpl->imageData+i*colorIpl->widthStep))[j*colorIpl->nChannels+1] = 0; 
	//			((uchar*)(colorIpl->imageData+i*colorIpl->widthStep))[j*colorIpl->nChannels+2] = 0; 
	//			//在已配准图像上标记
	//			((uchar*)(regColorIpl->imageData+i*regColorIpl->widthStep))[j*regColorIpl->nChannels+0] = 0; 
	//			((uchar*)(regColorIpl->imageData+i*regColorIpl->widthStep))[j*regColorIpl->nChannels+1] = 0; 
	//			((uchar*)(regColorIpl->imageData+i*regColorIpl->widthStep))[j*regColorIpl->nChannels+2] = 0; 
	//		}
	//	}
	//}
	//if(bShowMidImg) {
	//	//07显示配准前后的彩色图像
	//	cvShowImage("未配准的彩色图像", colorIpl);
	//	cvShowImage("已配准的彩色图像", regColorIpl);
	//}

	if(bShowMidImg) {
		//06显示精确的前景图
		cvShowImage("精确的前景图像", depthIpl);
	}

	//对掩膜图像进行下采样，并进行大半径中值滤波
	IplImage *smallMaskIpl = resizeImage(maskIpl, GF_SCALE);		
	cvReleaseImage(&maskIpl);
	cvSmooth(smallMaskIpl, smallMaskIpl, CV_MEDIAN, 7); //19
	if(bShowMidImg) {
		cvShowImage("掩膜图像中值滤波结果", smallMaskIpl);
	}

	//在中值滤波后的掩膜图像中定位人头的位置
	headRect = LocateHeadRegionRoughly(smallMaskIpl, B_LOCATE_HEAD);
	if(headRect.width == -1) {
		cvReleaseImage(&regColorIpl);
		cvReleaseImage(&smallMaskIpl);
		return NULL;
	}

	////仅用于显示中间结果，无实质性作用，可删除
	//if (bShowMidImg) {
	//	CvRect &rct = headRect;
	//	cvRectangle(smallMaskIpl, cvPoint(rct.x, rct.y), cvPoint(rct.x+rct.width, rct.y+rct.height), CV_RGB(0,0,128), 1);
	//	cvShowImage("小人头定位结果", smallMaskIpl);
	//}
	cvReleaseImage(&smallMaskIpl);
	return regColorIpl;
}