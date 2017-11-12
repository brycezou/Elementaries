#include "highgui.h"
#include "cv.h"
#include <fstream>
using namespace std;
using namespace cv;
#define NUM 17


typedef struct PT 
{
	int x;
	int y;
}PT;

//���ͼ��
void cvMySetMatEmpty(Mat &image)
{
	for (int i=0; i<image.rows; i++)
	{
		uchar *depthhead = image.ptr<uchar>(i);//point of the row i , one uchar indicates one color
		for (int j=0; j<image.cols; j++)
		{
			depthhead[3*j] = 0;
			depthhead[3*j+1] = 0;
			depthhead[3*j+2] = 0;
		}
	}
}
//�����������ߵĽǶ�
double getAtan(CvPoint pt1, CvPoint pt2)
{
	double dy=pt2.y-pt1.y;
	double dx=pt2.x-pt1.x;
	double basicTheta;
	if(dx==0.0)
	{
		if (dy>0.0)
			return 90;
		else if(dy<0.0)
			return -90;
		else
			return 200;
	}
	else if(dy==0.0)
	{
		if(dx>0.0)
			return 0;
		else if(dx<0.0)
			return 180;
	}
	else
		basicTheta=180/3.1415926535897*atan(dy/dx);

	if (dx<0 && dy>0)
		return basicTheta+180;
	else if(dx<0 && dy<0)
		return basicTheta-180;
	else
		return basicTheta;
}

int main(int argc, char** argv)
{
	if(argc!=2) return 0;
	ifstream infile;
	infile.open(argv[1]);
	int size;
	PT *pt;
	infile>>size;
	pt=new PT[size];
	int minX=500,minY=500,maxX=0,maxY=0;
	for (int i=0; i<size; i++)//����ԭʼ�켣�㲢�ҳ�����X��Y�����ͶӰ��Χ
	{
		infile>>pt[i].x>>pt[i].y;
		if(pt[i].x<minX) minX=pt[i].x;
		if(pt[i].y<minY) minY=pt[i].y;
		if(pt[i].x>maxX) maxX=pt[i].x;
		if(pt[i].y>maxY) maxY=pt[i].y;
	}
	Mat trajectoryImage;
	trajectoryImage.create(240, 320, CV_8UC3); 
	cvMySetMatEmpty(trajectoryImage);
	for (int i=0; i<size-1; i++)//����ԭʼ�켣����
	{
		line(trajectoryImage, cvPoint(pt[i].x, pt[i].y), cvPoint(pt[i+1].x, pt[i+1].y), cvScalar(255, 255, 255), 5); 
	}
	IplImage ipl_trajectory=trajectoryImage;
	cvShowImage("ipl_trajectory", &ipl_trajectory);//��ʾ��ԭʼ�켣����
	cvSaveImage("ipl_trajectory.jpg", &ipl_trajectory);//����ԭʼ�켣����ͼ

	/////
	CvRect rect;
	rect.x=minX;
	rect.y=minY;
	rect.width=maxX-minX;
	rect.height=maxY-minY;
	cvSetImageROI(&ipl_trajectory, rect);
	cvSaveImage("ipl_trajectory_ROI.jpg", &ipl_trajectory );//��������ROI���ͼ��
	cvShowImage("ipl_trajectory_ROI", &ipl_trajectory);//��ʾ����ROI���ͼ��
	IplImage *dst=0;
	dst = cvCreateImage(cvSize(20, 20), (&ipl_trajectory)->depth, (&ipl_trajectory)->nChannels);	//������С���Ŀ��ͼ��
	cvResize(&ipl_trajectory, dst, CV_INTER_LINEAR);
	cvSaveImage("11.jpg", dst );//������С���ͼ��
	cvShowImage("11", dst);
	cvReleaseImage(&dst);
	cvResetImageROI(&ipl_trajectory);


	/////�������Զ���Ĺ����ԭʼ���߽��в���
	int m=NUM;
	size=size-3;
	int r=size/m;
	cout<<m<<'\t'<<r<<endl;
	int A=r*m;
	int B=r*(m-1)+(m+1)/2;
	int C=r*(m-1)+m;
	int D=(r+1)*m;

	PT *sampledPT;
	sampledPT=new PT[NUM];
	for (int i=0; i<NUM; i++)
	{
		if (size>=A && size<B)
		{
			sampledPT[i]=pt[r*i+3];
		} 
		else if (size>=B && size<C)
		{
			switch (i%2)
			{
			case 0:
				sampledPT[i]=pt[(2*r+1)*i/2+3];
				break;
			case 1:
				sampledPT[i]=pt[(2*r+1)*(i-1)/2+r+3];
				break;
			}
		}
		else if (size>=C && size<D)
		{
			sampledPT[i]=pt[(r+1)*i+3];
		}
	}

	/////
	Mat sampledImage;//����ͼ��
	sampledImage.create(240, 320, CV_8UC3); 
	cvMySetMatEmpty(sampledImage);

	for (int i=0; i<NUM-1; i++)//�����������ͼ��
	{
		line(sampledImage, cvPoint(sampledPT[i].x, sampledPT[i].y), cvPoint(sampledPT[i+1].x, sampledPT[i+1].y), cvScalar(255, 255, 0), 5); 
		circle(sampledImage, cvPoint(sampledPT[i].x, sampledPT[i].y), 5, cvScalar(0, 0, 255), 2, 8, 0); 
		circle(trajectoryImage, cvPoint(sampledPT[i].x, sampledPT[i].y), 5, cvScalar(0, 0, 255), 2, 8, 0); 
		if (i==NUM-2)
		{
				circle(sampledImage, cvPoint(sampledPT[i+1].x, sampledPT[i+1].y), 5, cvScalar(0, 0, 255), 2, 8, 0); 
				circle(trajectoryImage, cvPoint(sampledPT[i+1].x, sampledPT[i+1].y), 5, cvScalar(0, 0, 255), 2, 8, 0); 
		}
	}
	IplImage ipl_sampled=sampledImage;
	cvShowImage("ipl_sampled", &ipl_sampled);//��ʾ�������ͼ��
	cvShowImage("ipl_trajectory_new", &ipl_trajectory);//��ʾ��ԭʼͼ����в�����ı��ͼ��
	cvSaveImage("ipl_sampled.jpg", &ipl_sampled );//����������ͼ��
	cvSaveImage("ipl_trajectory_new.jpg", &ipl_trajectory);//�����ԭʼͼ����в�����ı��ͼ��
	for (int i=0; i<NUM; i++)
	{
		cout<<sampledPT[i].x<<'\t'<<sampledPT[i].y<<endl;
	}

	cvWaitKey(0);
	cvDestroyAllWindows();
	delete pt;
	delete sampledPT;
	return 0;
}