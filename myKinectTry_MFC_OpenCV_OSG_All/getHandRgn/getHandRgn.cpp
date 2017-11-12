#include "highgui.h"
#include "cv.h"
using namespace std;


//使用ROI取得特定关心区域
int main(int argc, char** argv)
{
	IplImage *src=cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
	IplImage * gray=0;
	IplImage * binary=0;

	CvRect rect;
	rect.x=0;
	rect.y=0;
	rect.width=200;
	rect.height=200;
	cvSetImageROI(src, rect);

	gray=cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	cvCvtColor(src, gray, CV_BGR2GRAY); 
	binary=cvCreateImage(cvGetSize(gray), IPL_DEPTH_8U, 1);
	cvThreshold(gray, binary, 15, 255, CV_THRESH_OTSU | CV_THRESH_BINARY_INV);
	cvShowImage("dst1", src);
	cvShowImage("dst2", gray);
	cvShowImage("dst3", binary);

	cvResetImageROI(src);
	cvShowImage("src", src);

	cvWaitKey(0);
	cvReleaseImage(&src);
	cvReleaseImage(&gray);
	cvReleaseImage(&binary);
	cvDestroyAllWindows();

	return 0;
}