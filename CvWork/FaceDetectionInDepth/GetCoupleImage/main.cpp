#include "kMeansClustering.h"


//从深度视频和彩色视频中保存对应的图像
int main(int argc, char **argv)
{
	if(argc != 3)
	{
		cout<<"only two argument \"[depth video] [color video]\" required"<<endl;	
		system("pause");	return -1;
	}
	CvCapture *capDepth = cvCreateFileCapture(argv[1]);
	if (NULL == capDepth)
	{
		cout<<"depth video is invalid"<<endl;
		system("pause");	return -1;
	}
	CvCapture *capColor = cvCreateFileCapture(argv[2]);
	if (NULL == capColor)
	{
		cout<<"color video is invalid"<<endl;
		system("pause");	return -1;
	}

	cvNamedWindow("clusteredResult");
	IplImage *frame_depth_3c = cvQueryFrame(capDepth);
	IplImage *depth_1c = cvCreateImage(cvSize(640,480), 8, 1);
	IplImage *result_3c = cvCreateImage(cvSize(640,480), 8, 3);
	IplImage *frame_color_3c = cvQueryFrame(capColor);

	int index = 0;
	while ((frame_depth_3c != NULL) && (frame_color_3c != NULL))
	{
		cvCvtColor(frame_depth_3c, depth_1c, CV_BGR2GRAY);
		kMeansClustering(depth_1c, result_3c);
		cvShowImage("clusteredResult", result_3c);
		if (cvWaitKey(1) == 27)		
			break;
		else if(cvWaitKey(1) == 's')
		{
			index++;
			char strTemp[50];
			sprintf(strTemp, "%03d_depth.bmp", index);
			cvSaveImage(strTemp, depth_1c);
			sprintf(strTemp, "%03d_color.bmp", index);
			cvSaveImage(strTemp, frame_color_3c);
		}
		cvWaitKey(20);
		frame_depth_3c = cvQueryFrame(capDepth);
		frame_color_3c = cvQueryFrame(capColor);
	}

	cvReleaseImage(&result_3c);
	cvReleaseImage(&depth_1c);
	//cvReleaseImage(&frame_color_3c);
	//cvReleaseImage(&frame_depth_3c);
	cvReleaseCapture(&capColor);
	cvReleaseCapture(&capDepth);
	return 0;
}