#include <opencv2/opencv.hpp>


//根据参数homography对输入的彩色图像srcColor进行变换
//返回值为变换后的图像，其尺寸与输入图像srcColor相同
IplImage* RegisterColor2Depth(IplImage *srcColor, CvMat *homography)
{
	int width = srcColor->width;
	int height = srcColor->height;
	IplImage *tmpRgb = cvCreateImage(cvSize(width*1.5, height*1.5), srcColor->depth, srcColor->nChannels);
	cvWarpPerspective(srcColor, tmpRgb, homography);
	cvSetImageROI(tmpRgb, cvRect(1, 1, width, height));
	IplImage *dstColor = cvCloneImage(srcColor);
	cvCopy(tmpRgb, dstColor);
	cvResetImageROI(tmpRgb);
	cvReleaseImage(&tmpRgb);
	return dstColor;
}
