#include "viCardRecogLib.h"


CvRect gOrgRectBoxArray[NUM_BOX_ORG];			//ÿ������Ĵ���λ�ã���������
CvRect gAccurateRectBoxArray[NUM_BOX_ORG];	//ÿ�������е����������λ�ã��������
CvRect gAbsoluteBoxRectArray[NUM_BOX_ORG];	//��ʵgAbsoluteBoxRectArray�д洢�Ĳ���������CvRect�ṹ�����Ǹýṹ�����ϽǺ����½ǵ�����ֵ
int gBlobFilterSize[NUM_BOX_ORG] = {3, 10, 60, 2, 10, 10, 10, 5, 10};

CvRect gEveryNumRect[18];			//���֤������ÿһ�����ֵ�׼ȷ����
IplImage *iplTemplateImgArray[11];	//ÿһ�����ֵ�ƥ��ʶ��ģ��ͼ��
char strNumberRecogResult[19];		//���֤����ʶ����


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
			cvShowImage("�ָ���", idCardCutImg);
			cvReleaseImage(&idCardCutImg);
		}
		else
			cout<<"����������ͼƬ!"<<endl<<endl;

		cvReleaseImage(&srcCaptured);
		cvWaitKey(0);
	}

	cvDestroyAllWindows();

	return 0;
}
