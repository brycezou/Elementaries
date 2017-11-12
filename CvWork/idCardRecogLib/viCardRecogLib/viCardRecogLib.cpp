#include "viCardRecogLib.h"


//�õ�����֤���������ֵͼ����ÿһ�����ֵ�׼ȷ����
//�������˳�򱣴���gEveryNumRect[18]��
void GetEveryNumRect(IplImage *srcBinary)
{
	CBlobResult blobs;
	CBlob *currentBlob;

	blobs = CBlobResult( srcBinary, NULL, 0 );											//Ѱ�Ҷ�ֵͼ�е�blobs
	blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, 2);	
	if (blobs.GetNumBlobs()==18)
	{
		for (int j = 0; j < blobs.GetNumBlobs(); j++ )
		{
			currentBlob = blobs.GetBlob(j);
			gEveryNumRect[j]=currentBlob->GetBoundingBox();						//�õ���ǰ��ͨ����������
		}
		//cvShowImage("binary", srcBinary);

		for (int i=0; i<17; i++)		//ð������
		{
			for (int j=0; j<17-i; j++)
			{
				if (gEveryNumRect[j].x>gEveryNumRect[j+1].x)
				{
					CvRect temp=gEveryNumRect[j];
					gEveryNumRect[j]=gEveryNumRect[j+1];
					gEveryNumRect[j+1]=temp;
				}
			}
		}
	}
	else
	{
		gEveryNumRect[0].x=-1;
	}
}

//ʶ������֤�����е�ÿһ������
char RecognizeEveryNumber(IplImage *img)
{
	int minDistance=60000;
	int minIndex=0;

	for (int i=0; i<11; i++)
	{
		int sumDelt=0;
		for (int j=0; j<img->height; j++)
		{
			for (int k=0; k<img->width; k++)
			{
				sumDelt+=abs(cvGet2D(iplTemplateImgArray[i], j, k).val[0]-cvGet2D(img, j, k).val[0]);
			}
		}
		if (sumDelt<minDistance)
		{
			minDistance=sumDelt;
			minIndex=i;
		}
	}
	switch (minIndex)
	{
	case 0:  return '0';
	case 1:  return '1';
	case 2:  return '2';
	case 3:  return '3';
	case 4:  return '4';
	case 5:  return '5';
	case 6:  return '6';
	case 7:  return '7';
	case 8:  return '8';
	case 9:  return '9';
	case 10:  return 'X';
	}
	return '\0';
}

void GetCardNumber(IplImage *idCardCutImg)
{
	for (int k=0; k<NUM_BOX_ORG; k++)
	{
		if (k==NUMBER_ORG)		//���������֤������ʶ��֮
		{
			CvRect rtNumber;		
			rtNumber.x=gAbsoluteBoxRectArray[k].x;
			rtNumber.y=gAbsoluteBoxRectArray[k].y;
			rtNumber.width=gAbsoluteBoxRectArray[k].width-gAbsoluteBoxRectArray[k].x+1;
			rtNumber.height=gAbsoluteBoxRectArray[k].height-gAbsoluteBoxRectArray[k].y;
			cvSetImageROI(idCardCutImg, rtNumber);

			IplImage *bin4Recognize=cvCreateImage(cvGetSize(idCardCutImg), IPL_DEPTH_8U, 1);
			getBinaryImageOTSU(idCardCutImg, bin4Recognize);
			GetEveryNumRect(bin4Recognize);
			if (gEveryNumRect[0].x==-1)
			{
				cvReleaseImage(&bin4Recognize);
				cvResetImageROI(idCardCutImg);
				return;
			}
			IplImage *resizedNumberImg=cvCreateImage(cvSize(12, 18), IPL_DEPTH_8U, 1);
			for (int j=0; j<18; j++)
			{
				cvSetImageROI(bin4Recognize, gEveryNumRect[j]);
				cvResize(bin4Recognize, resizedNumberImg, CV_INTER_LINEAR);
				strNumberRecogResult[j]=RecognizeEveryNumber(resizedNumberImg);
				cvResetImageROI(bin4Recognize);
			}
			strNumberRecogResult[18]='\0';

			cvReleaseImage(&resizedNumberImg);
			cvReleaseImage(&bin4Recognize);

			cvResetImageROI(idCardCutImg);
		}

		cvRectangle(idCardCutImg, cvPoint(gAbsoluteBoxRectArray[k].x, gAbsoluteBoxRectArray[k].y), cvPoint(gAbsoluteBoxRectArray[k].width, gAbsoluteBoxRectArray[k].height), cvScalar(0, 0, 255), 1);	
	}
}

//��API, srcCapturedΪ����ԭʼͼ��, ���ش������ͼ��
//����ʶ�������֤�����������strNumberRecogResult��
IplImage* RecognizeIDCardNumber(IplImage *srcCaptured)
{
	const char* cascade_name ="haarcascade_frontalface_alt.xml";																//������������
	CvHaarClassifierCascade *cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );		//��������������
	if(!cascade)  return NULL;

	iplTemplateImgArray[0]=cvLoadImage("number_templates/0.jpg", CV_LOAD_IMAGE_GRAYSCALE);		//����������֤����ʶ�������ģ��ͼ���ȼ����ڴ�
	iplTemplateImgArray[1]=cvLoadImage("number_templates/1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	iplTemplateImgArray[2]=cvLoadImage("number_templates/2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	iplTemplateImgArray[3]=cvLoadImage("number_templates/3.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	iplTemplateImgArray[4]=cvLoadImage("number_templates/4.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	iplTemplateImgArray[5]=cvLoadImage("number_templates/5.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	iplTemplateImgArray[6]=cvLoadImage("number_templates/6.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	iplTemplateImgArray[7]=cvLoadImage("number_templates/7.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	iplTemplateImgArray[8]=cvLoadImage("number_templates/8.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	iplTemplateImgArray[9]=cvLoadImage("number_templates/9.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	iplTemplateImgArray[10]=cvLoadImage("number_templates/x.jpg", CV_LOAD_IMAGE_GRAYSCALE);


	IplImage *idCardCutImg=GetPreDefinedAboutBoxes(srcCaptured, cascade);
	if (idCardCutImg)
	{
		GetAbsoluteAccurateBoxRects(idCardCutImg);		//�õ��������ڵľ�ȷ����, �洢��gAbsoluteBoxRectArray[]��
		GetCardNumber(idCardCutImg);
		if (gEveryNumRect[0].x==-1)
		{
			cvReleaseImage(&idCardCutImg);
			for (int i=0; i<11; i++)
			{
				cvReleaseImage(&(iplTemplateImgArray[i]));
			}
			cvReleaseHaarClassifierCascade( &cascade );
			return NULL;
		}
		else
		{
			for (int i=0; i<11; i++)
			{
				cvReleaseImage(&(iplTemplateImgArray[i]));
			}
			cvReleaseHaarClassifierCascade( &cascade );

			return idCardCutImg;
		}
	}
	else
	{
		for (int i=0; i<11; i++)
		{
			cvReleaseImage(&(iplTemplateImgArray[i]));
		}
		cvReleaseHaarClassifierCascade( &cascade );
		return NULL;
	}
}
