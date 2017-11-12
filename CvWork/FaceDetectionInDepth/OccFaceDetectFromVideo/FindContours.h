#include <opencv2/opencv.hpp>


//先提取smallMaskIpl的轮廓,
//再进行傅里叶变换和逆变换, 进行平滑
void FindContours(const IplImage *smallMaskIpl, bool bShowMidImg)
{
	//得到掩膜图像smallMaskIpl的轮廓点
	IplImage *counterIpl = findMaxContours(smallMaskIpl);
	//09显示掩膜图像的外轮廓点
	if(bShowMidImg)
		cvShowImage("掩膜图像的轮廓点", counterIpl);
	//for (int i = 0; i < pContoursList[0].x; i++)
	//	cout<<pContoursList[i+1].x<<", "<<pContoursList[i+1].y<<endl;

	//计算掩膜图像边界点pContoursList的傅里叶描述子fourierFactor
	//重构后得到平滑的边界轮廓线，保存在pContoursList中
	CvPoint2D64f *fourierFactor = calcuFourierFactor();
	invrsFourierTransform(fourierFactor);
	delete []fourierFactor;

	if(bShowMidImg) {
		for (int i = 0; i < pContoursList[0].x-1; i++)
			cvLine(counterIpl, cvPoint(pContoursList[i+1].x, pContoursList[i+1].y),cvPoint(pContoursList[i+2].x, pContoursList[i+2].y), CV_RGB(0,0,255), 1);
		//10显示平滑后的掩膜图像外轮廓点
		cvShowImage("平滑后的轮廓点", counterIpl);
	}

	cvReleaseImage(&counterIpl);
}
