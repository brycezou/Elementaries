#include "highgui.h"
#include "cv.h"
using namespace std;


//得到特定颜色的区域
int main(int argc, char** argv)
{
	if(argc!=2) return 0;

	CvScalar color=cvScalar(0, 0, 0); 
	CvScalar stemp;
	CvScalar newcolor=cvScalar(0, 255, 0); 

	IplImage *src=cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
	IplImage * binary=0;
	binary=cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);

	for (int i=0; i<binary->height; i++)
	{
		for (int j=0; j<binary->width; j++)
		{
			((uchar *)(binary->imageData+i*binary->widthStep))[j]=0;
		}
	}

	for (int i=0; i<src->height; i++)
	{
		for (int j=0; j<src->width; j++)
		{
			stemp=cvGet2D(src, i, j);
			if (abs(stemp.val[0]-color.val[0])<=20 && abs(stemp.val[1]-color.val[1])<=20 && abs(stemp.val[2]-color.val[2])<=20)
			{
				cvSet2D(src, i, j, newcolor);
				((uchar *)(binary->imageData+i*binary->widthStep))[j]=255;
			}
		}
	}
	cvShowImage("src", src);
	cvShowImage("binary", binary);

	cvWaitKey(0);
	cvReleaseImage(&src);
	cvReleaseImage(&binary);
	cvDestroyAllWindows();
	return 0;
}