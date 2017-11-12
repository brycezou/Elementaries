#include "highgui.h"
#include "cv.h"
#include <fstream>
#include <Windows.h>
using namespace std;
using namespace cv;
#define NUM 17


typedef struct PT 
{
	int x;
	int y;
}PT;

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


	////////////////////////////
	Mat trajectoryImage;
	trajectoryImage.create(240, 320, CV_8UC3); 
	IplImage ipl_trajectory;
	CvRect rect;
	IplImage *dst=0;
	////////////////////////////


	ifstream infile;
	infile.open(argv[1]);
	int size;
	PT *pt;
	PT *sampledPT;
	ofstream outfile;
	outfile.open("outSampled.txt");


	////////////////////////////
	CvScalar stemp;
	CvScalar blackcolor=cvScalar(0, 0, 0); 
	////////////////////////////

	while(!infile.eof())
	{
		infile>>size;
		pt=new PT[size];
		int minX=500,minY=500,maxX=0,maxY=0;////////////////////////////
		for (int i=0; i<size; i++)
		{
			infile>>pt[i].x>>pt[i].y;

			////////////////////////////
			if(pt[i].x<minX) minX=pt[i].x;
			if(pt[i].y<minY) minY=pt[i].y;
			if(pt[i].x>maxX) maxX=pt[i].x;
			if(pt[i].y>maxY) maxY=pt[i].y;
			////////////////////////////
		}
	

		////////////////////////////
		cvMySetMatEmpty(trajectoryImage);
		for (int i=0; i<size-1; i++)
		{
			line(trajectoryImage, cvPoint(pt[i].x, pt[i].y), cvPoint(pt[i+1].x, pt[i+1].y), cvScalar(255, 255, 255), 5); 
		}
		ipl_trajectory=trajectoryImage;
		rect.x=minX;
		rect.y=minY;
		rect.width=maxX-minX;
		rect.height=maxY-minY;
		cvSetImageROI(&ipl_trajectory, rect);
		dst = cvCreateImage(cvSize(20, 20), (&ipl_trajectory)->depth, (&ipl_trajectory)->nChannels);	//构造目标图象
		cvResize(&ipl_trajectory, dst, CV_INTER_LINEAR);
		////////////////////////////


		int m=NUM;
		size=size-3;
		int r=size/m;
		cout<<m<<'\t'<<r<<endl;
		int A=r*m;
		int B=r*(m-1)+(m+1)/2;
		int C=r*(m-1)+m;
		int D=(r+1)*m;

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

		outfile<<"zaluan"<<'\t';
		for (int i=0; i<NUM-1; i++)
		{
			outfile<<i+1<<":"<<(int)getAtan(cvPoint(sampledPT[i].x, sampledPT[i].y), cvPoint(sampledPT[i+1].x, sampledPT[i+1].y))<<'\t';
		}

		////////////////////////////
		int mm=17;
		for (int i=0;i<dst->height;i++)
		{
			for (int j=0;j<dst->width;j++)
			{
				stemp=cvGet2D(dst, i, j);
				if (blackcolor.val[0]==stemp.val[0] && blackcolor.val[1]==stemp.val[1] && blackcolor.val[2]==stemp.val[2])
				{
					outfile<<mm++<<":"<<stemp.val[0]<<'\t';
				}
				else
					outfile<<mm++<<":"<<255<<'\t';
			}
		}
		////////////////////////////

		outfile<<endl;

		//for (int i=0; i<NUM; i++)
		//{
		//	cout<<sampledPT[i].x<<'\t'<<sampledPT[i].y<<endl;
		//}

		delete pt;
		delete sampledPT;
		cvReleaseImage(&dst);////////////////////////////
		cvResetImageROI(&ipl_trajectory);////////////////////////////
	}

	cvWaitKey(0);
	cvDestroyAllWindows();
	outfile.close();
	while (true)
	{
		Sleep(10000);
	}
	return 0;
}