#include <opencv2/opencv.hpp>
#include "BlobResult.h"
#include "blob.h"
#include "kMeansClustering.h"


//在掩膜图像maskIpl中寻找面积最大的连通域，返回标记图像
IplImage* findMaxObject(IplImage *maskIpl)
{
	CBlobResult blobs;
	CBlob *currentBlob;

	IplImage *blobRGB = cvCreateImage(cvGetSize(maskIpl), IPL_DEPTH_8U, 3);
	blobs = CBlobResult(maskIpl, NULL, 0);
	blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, 20);
	cvMerge(maskIpl,  maskIpl,  maskIpl, NULL, blobRGB);

	int blobNumber = 0;
	for (int j = 0; j < blobs.GetNumBlobs(); j++)
	{
		currentBlob = blobs.GetBlob(j);
		CvRect rct = currentBlob->GetBoundingBox();
		int wi = rct.width;
		int he = rct.height;
		int Height = maskIpl->height;
		int Width = maskIpl->width;
		if (wi*he > Width*Height/20)			
		{
			currentBlob->FillBlob(maskIpl, CV_RGB(0, 0, 255));
			currentBlob->FillBlob( blobRGB, CV_RGB(0, 0, 255));
			cvRectangle(blobRGB, cvPoint(rct.x, rct.y), cvPoint(rct.x+rct.width, rct.y+rct.height), cvScalar(0, 255, 0), 1);
			blobNumber++;
		}
		else
			currentBlob->FillBlob(maskIpl, CV_RGB(0, 0, 0));
	}

	if (blobNumber==0)
	{
		cvReleaseImage(&blobRGB);
		return NULL;
	}
	return blobRGB;
}

//在深度图像中寻找前景目标
IplImage* getForegroundMask(IplImage *depthIpl, bool bShowMidImg)
{
	//根据深度值对深度图像进行聚类，掩膜保存在maskIpl，聚类结果保存在clusteredIpl
	int w = depthIpl->width;
	int h = depthIpl->height;
	IplImage *clusteredIpl = cvCreateImage(cvSize(w, h), 8, 3);
	IplImage *maskIpl = kMeansClustering(depthIpl, clusteredIpl);
	if(bShowMidImg) {
		//02显示聚类结果图
		cvShowImage("深度图像聚类结果图", clusteredIpl);
		cvShowImage("最原始的掩膜图像", maskIpl);
	}
	cvReleaseImage(&clusteredIpl);

	//对掩膜图像进行闭运算，消除孔洞
	IplImage *temp = cvCreateImage(cvSize(w, h), 8, 1);
	cvMorphologyEx(maskIpl, maskIpl, temp, NULL, CV_MOP_CLOSE, 10);
	cvReleaseImage(&temp);
	if(bShowMidImg) {
		//03显示掩膜图像
		cvShowImage("形态学运算后的掩膜图像", maskIpl);
	}

	//在掩膜图像中寻找面积最大的连通域，同时修正掩膜
	IplImage *blobIpl = findMaxObject(maskIpl);
	if(blobIpl == NULL)	
	{
		cvReleaseImage(&maskIpl);
		return NULL;
	}
	//04显示掩膜图像的最大连通域提取结果
	if(bShowMidImg)	
		cvShowImage("掩膜图像的最大连通域", blobIpl);
	cvReleaseImage(&blobIpl);

	//根据掩膜maskIpl和原始深度图像depthIpl获得精确的前景图像，保存到depthIpl
	for (int i = 0; i < depthIpl->height; i++)
	{
		for (int j = 0; j < depthIpl->width; j++)
		{
			int maskVal = ((uchar *)(maskIpl->imageData+i*maskIpl->widthStep))[j];
			int pixVal = ((uchar *)(depthIpl->imageData+i*depthIpl->widthStep))[j];
			if (maskVal < 32)	//exactly when maskVal == 0 
				((uchar *)(depthIpl->imageData+i*depthIpl->widthStep))[j] = 0;
			else if (pixVal > 160 || pixVal < 32)	//eliminate the effect of over-morphology
			{
				((uchar *)(depthIpl->imageData+i*depthIpl->widthStep))[j] = 0;
				//modify mask image
				//暂时注释掉这句, 实验表明mask不需要太精确，只要其轮廓即可!
				//注释于2015-11-12
				//((uchar *)(maskIpl->imageData+i*maskIpl->widthStep))[j] = 0;
			}
		}
	}
	//05显示修正后的精确前景掩膜
	if(bShowMidImg){
		cvShowImage("精确的掩膜图像", maskIpl);
	}

	return maskIpl;
}