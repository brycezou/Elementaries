#include "viCardRecogLib.h"


CvRect gOrgRectBoxArray[NUM_BOX_ORG];			//每个区域的大致位置，绝对坐标
CvRect gAccurateRectBoxArray[NUM_BOX_ORG];	//每个区域中的文字区域的位置，相对坐标
CvRect gAbsoluteBoxRectArray[NUM_BOX_ORG];	//其实gAbsoluteBoxRectArray中存储的不是真正的CvRect结构，而是该结构的左上角和右下角的坐标值
int gBlobFilterSize[NUM_BOX_ORG] = {3, 10, 60, 2, 10, 10, 10, 5, 10};

CvRect gEveryNumRect[18];			//身份证号码中每一个数字的准确区域
IplImage *iplTemplateImgArray[11];	//每一个数字的匹配识别模板图像
char strNumberRecogResult[19];		//身份证号码识别结果


int main(int argc, char **argv)
{
	if (argc==1)	return -1;			

	IplImage *srcCaptured=0;
	IplImage *idCardCutImg=0;

	for (int i=0; i<argc-1; i++)
	{
		srcCaptured=cvLoadImage(argv[i+1], CV_LOAD_IMAGE_COLOR);												
		idCardCutImg=RecognizeIDCardNumber(srcCaptured);
		if (idCardCutImg)
		{
			cout<<strNumberRecogResult<<endl<<endl;
			cvShowImage("分割结果", idCardCutImg);
			cvReleaseImage(&idCardCutImg);
		}
		else
			cout<<"请重新拍摄图片!"<<endl<<endl;

		cvReleaseImage(&srcCaptured);
		cvWaitKey(0);
	}

	cvDestroyAllWindows();

	return 0;
}
