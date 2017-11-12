#include "viMatrixCalculator.h"


int main(int argc, char** argv)
{
	IplImage *srcRGB = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
	char *strResult = getJsonMatrix(srcRGB);
	cout<<strResult<<endl;
	cvReleaseImage(&srcRGB);
	cvWaitKey(0);
	return 0;
}