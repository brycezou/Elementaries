#include "obtainForegroundMask.h"
#include "FourierTransform.h"
#include "LocateHeadRegion.h"
#include "registerColor2Depth.h"
#include "FindContours.h"
#include "GetHeadRegionRect.h"
#include <fstream>
#include "svm.h"

using namespace std;


//�ĸ��߼��ֿ���ڵ����, 1Ϊ�ڵ�, 0Ϊ���ڵ�
int EYES_OCCLUDED = 1;
int NOSE_OCCLUDED = 0;
int MOUTH_OCCLUDED = 1;
int CHIN_OCCLUDED = 0; 

//�����ͼ��depthIpl����׼��Ĳ�ɫͼ��regColorIpl��
//��ǳ���������headRect
//˵����ֱ����ԭͼ�ϱ��
void showHeadRectResult(IplImage *depthIpl, IplImage *regColorIpl, CvRect &headRect)
{
	cvRectangle(depthIpl, cvPoint(2*headRect.x, 2*headRect.y), cvPoint(2*(headRect.x+headRect.width), 2*(headRect.y+headRect.height)), CV_RGB(0,0,128), 2);
	cvShowImage("�����ͷ��λ���", depthIpl);
	cvRectangle(regColorIpl, cvPoint(2*headRect.x, 2*headRect.y), cvPoint(2*(headRect.x+headRect.width), 2*(headRect.y+headRect.height)), CV_RGB(255,0,0), 2);
	cvShowImage("��ɫ��ͷ��λ���", regColorIpl);
}

//��������ͼ��src���µĳߴ�
//���ź�Ŀ��ΪnewWidth
//���ź�ĸ߶�ΪnewHeight
//˵�����᷵���µ�ͼ��
IplImage* resizeInputImage(IplImage *src, int newWidth, int newHeight)
{
	IplImage *newSizeImg = cvCreateImage(cvSize(newWidth, newHeight), src->depth, src->nChannels);
	cvResize(src, newSizeImg, CV_INTER_LINEAR);
	return newSizeImg;
}

//��48x56������������Ԥ���������
CvRect EYES_RECT	= cvRect(6, 16, 36, 16);		//�۾�����
CvRect NOSE_RECT	= cvRect(6, 24, 36, 16);		//��������
CvRect MOUTH_RECT	= cvRect(6, 32, 36, 16);	//�������
CvRect CHIN_RECT = cvRect(6, 48, 36, 8);			//�°�����

//�߼������ֿ����������
//��ɫͼ��Ĳ�ɫֱ��ͼ, ��ɫͼ��ĻҶ�ֱ��ͼ
//��ɫͼ���Haar����, ���ͼ���Haar����
int f_vc[4172];	

//�����ĸ��߼������ֿ������
ofstream ofile_eyes("eyes.txt", ios::app);
ofstream ofile_nose("nose.txt", ios::app);
ofstream ofile_mouth("mouth.txt", ios::app);
ofstream ofile_chin("chin.txt", ios::app);

//�õ�ͼ��color_img�Ĳ�ɫֱ��ͼ(ÿ��ͨ��ȡ16���Ҷ�), ������f_vc�����[0:4095]λ
//�ͻҶ�ֱ��ͼ(64���Ҷ�), ������f_vc�����[4096:4159]λ
void getColorAndGrayHistogram(IplImage *color_img)
{
	memset(f_vc, 0, sizeof(int)*4172);
	for (int i = 0; i < color_img->height; i++)
	{
		for (int j = 0; j < color_img->width; j++)
		{
				int rr = ((uchar*)(color_img->imageData+i*color_img->widthStep))[j*color_img->nChannels+0]; 
				int gg = ((uchar*)(color_img->imageData+i*color_img->widthStep))[j*color_img->nChannels+1]; 
				int bb = ((uchar*)(color_img->imageData+i*color_img->widthStep))[j*color_img->nChannels+2]; 
				f_vc[rr/16*256+gg/16*16+bb/16]++;							//ͳ�Ʋ�ɫֱ��ͼ [0:4095]
				f_vc[((int)(0.30*rr+0.59*gg+0.11*bb))/4+4096]++;	//ͳ�ƻҶ�ֱ��ͼ [4096:4159]
		}
	}
}

//�õ�ͼ��color_roi��������д���ı��ļ���
void getFeatureVectors4Color(IplImage *color_roi)
{
	//�۾�����
	cvSetImageROI(color_roi, EYES_RECT);
	//cvSaveImage("eyes.jpg", color_roi);
	getColorAndGrayHistogram(color_roi);
	ofile_eyes<<EYES_OCCLUDED<<" ";
	for (int i = 0; i < 4172; i++)
		ofile_eyes<<i+1<<":"<<f_vc[i]<<" ";
	ofile_eyes<<endl;
	cvResetImageROI(color_roi);

	//��������
	cvSetImageROI(color_roi, NOSE_RECT);
	//cvSaveImage("nose.jpg", color_roi);
	getColorAndGrayHistogram(color_roi);
	ofile_nose<<NOSE_OCCLUDED<<" ";
	for (int i = 0; i < 4172; i++)
		ofile_nose<<i+1<<":"<<f_vc[i]<<" ";
	ofile_nose<<endl;
	cvResetImageROI(color_roi);

	//�������
	cvSetImageROI(color_roi, MOUTH_RECT);
	//cvSaveImage("mouth.jpg", color_roi);
	getColorAndGrayHistogram(color_roi);
	ofile_mouth<<MOUTH_OCCLUDED<<" ";
	for (int i = 0; i < 4172; i++)
		ofile_mouth<<i+1<<":"<<f_vc[i]<<" ";
	ofile_mouth<<endl;
	cvResetImageROI(color_roi);

	//�°�����
	cvSetImageROI(color_roi, CHIN_RECT);
	//cvSaveImage("chin.jpg", color_roi);
	getColorAndGrayHistogram(color_roi);
	ofile_chin<<CHIN_OCCLUDED<<" ";
	for (int i = 0; i < 4172; i++)
		ofile_chin<<i+1<<":"<<f_vc[i]<<" ";
	ofile_chin<<endl;
	cvResetImageROI(color_roi);
}

void generateFeatureFiles(IplImage *depthIpl, IplImage *regColorIpl, CvRect &headRect)
{
	//�����������Ȥ����
	CvRect rect_roi = cvRect(2*headRect.x, 2*headRect.y, 2*headRect.width, 2*headRect.height);

	//���������ͼ�����ŵ�48*56����
	cvSetImageROI(depthIpl, rect_roi);
	IplImage *depth_roi = resizeInputImage(depthIpl, 48, 56);
	cvResetImageROI(depthIpl);

	//����ɫ����ͼ�����ŵ�48*56����
	cvSetImageROI(regColorIpl, rect_roi);
	IplImage *color_roi = resizeInputImage(regColorIpl, 48, 56);
	cvResetImageROI(regColorIpl);

	//cvShowImage("depth_roi", depth_roi);
	//cvShowImage("color_roi", color_roi);

	char ch = cvWaitKey(0);
	switch(ch)
	{
	case 's':
		//�����ɫ�������������������
		getFeatureVectors4Color(color_roi);
		cout<<"saved"<<endl;
		break;
	default:
		cout<<"not saved"<<endl;
		break;
	}

	cvReleaseImage(&color_roi);
	cvReleaseImage(&depth_roi);
}


int main(int argc, char **argv)
{
	//�������Ƶ�Ͳ�ɫ��ƵΪ����, ��ȡ�����ļ�
	if(argc != 3) {
		cout<<"only two argument \"[depth video] [color video]\" required"<<endl;	
		system("pause");	return -1;
	}
	//��ȡ�����Ƶ
	CvCapture *capDepth = cvCreateFileCapture(argv[1]);
	if (NULL == capDepth) {
		cout<<"depth video is invalid"<<endl;
		system("pause");	return -1;
	}
	//��ȡ��ɫ��Ƶ
	CvCapture *capColor = cvCreateFileCapture(argv[2]);
	if (NULL == capColor) {
		cout<<"color video is invalid"<<endl;
		system("pause");	return -1;
	}
	//����4���߼��ֿ���ڵ����
	while(true) {
		cout<<"Eyes? Please input 1 for occluded or 0 for not."<<endl;
		cin>>EYES_OCCLUDED;
		cout<<"Nose? Please input 1 for occluded or 0 for not."<<endl;
		cin>>NOSE_OCCLUDED;
		cout<<"Mouth? Please input 1 for occluded or 0 for not."<<endl;
		cin>>MOUTH_OCCLUDED;
		cout<<"Chin? Please input 1 for occluded or 0 for not."<<endl;
		cin>>CHIN_OCCLUDED;
		cout<<"Please confirm: ";
		cout<<EYES_OCCLUDED<<", "<<NOSE_OCCLUDED<<", "<<MOUTH_OCCLUDED<<", "<<CHIN_OCCLUDED;
		cout<<" (y/~y?)  ";
		char ch;
		bool ifgon = false;
		cin>>ch;
		switch(ch)
		{
		case 'y':
			ifgon = true;
			break;
		default:
			ifgon = false;
			break;
		}
		if(ifgon)
			break;
		else
			cout<<endl<<endl;
	}
	//ע��:����������Ƶ֡Ϊ3ͨ��, ���㷨��ʹ�õ��ǵ�ͨ��
	IplImage *frame_depth_3c = cvQueryFrame(capDepth);
	IplImage *depthIpl = cvCreateImage(cvSize(640,480), 8, 1);
	IplImage *colorIpl = cvQueryFrame(capColor);
	IplImage *depth2show = NULL;
	IplImage *color2show = NULL;
	CvRect headRect;
	while((frame_depth_3c != NULL) && (colorIpl != NULL))
	{
		//����ͨ���������Ƶ֡ת���ɵ�ͨ���ĻҶ�ͼ��
		cvCvtColor(frame_depth_3c, depthIpl, CV_BGR2GRAY);
		depth2show = cvCloneImage(depthIpl);
		IplImage *regColorIpl = GetHeadRegionRect(depthIpl, colorIpl, headRect, false);
		if (regColorIpl != NULL)
		{
			color2show = cvCloneImage(regColorIpl);
			showHeadRectResult(depth2show, color2show, headRect);
			generateFeatureFiles(depthIpl, regColorIpl, headRect);
			cvReleaseImage(&regColorIpl);
			cvReleaseImage(&color2show);
		}
		cvReleaseImage(&depth2show);
		frame_depth_3c = cvQueryFrame(capDepth);
		colorIpl = cvQueryFrame(capColor);
	}
	cout<<"finished!"<<endl;
	cvReleaseImage(&colorIpl);
	cvReleaseImage(&depthIpl);
	cvReleaseImage(&frame_depth_3c);

	return 0;
}
