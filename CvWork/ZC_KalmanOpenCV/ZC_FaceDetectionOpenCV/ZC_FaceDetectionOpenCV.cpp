#include <Windows.h>
#include <iostream>
#include <fstream>
#include <highgui.h>
#include <cv.h>
using namespace std;


#define STATE_NUM			4		//x, y, dx, dy
#define MEASURE_NUM	2		//x, y


CvKalman* kalman = 0;
CvMat* measure = 0;
const CvMat* prediction = 0;
CvPoint realPoint = cvPoint(160, 120);
CvHaarClassifierCascade *cascade = 0;
//ofstream outfile;

void ZcFaceDetection(IplImage*img)
{
	CvMemStorage*storage = cvCreateMemStorage(0);
	cvClearMemStorage(storage);
	IplImage*gray = cvCreateImage(cvSize(img->width, img->height), 8, 1);
	cvCvtColor(img, gray, CV_BGR2GRAY);
	cvEqualizeHist(gray, gray);        

	CvSeq *faces = cvHaarDetectObjects(gray, cascade, storage, 1.2, 2, 0, cvSize(40, 40));
	for(int i=0; i < (faces?faces->total:0); i++)                      
	{
		CvRect*rtemp=(CvRect*)cvGetSeqElem(faces, i);
		cvRectangle(img, cvPoint(rtemp->x, rtemp->y), cvPoint(rtemp->x+rtemp->width, rtemp->y+rtemp->height), cvScalar(255, 0, 0), 2);
		realPoint = cvPoint(rtemp->x+rtemp->width/2, rtemp->y+rtemp->height/2);
		cvCircle(img, realPoint, 5, CV_RGB(0, 0, 255), 3);	     
	}     

	prediction = cvKalmanPredict(kalman, 0);
	CvPoint predictPoint = cvPoint((int)prediction->data.fl[0], (int)prediction->data.fl[1]);		//x, y
	cvCircle(img, predictPoint, 5, CV_RGB(255, 0, 0), 3);	

	//outfile<<realPoint.x<<'\t'<<realPoint.y<<'\t'<<(int)prediction->data.fl[0]<<'\t'<<(int)prediction->data.fl[1]<<endl;

	//3.update measure
	measure->data.fl[0] = (float)realPoint.x;
	measure->data.fl[1] = (float)realPoint.y;
	//4.update
	cvKalmanCorrect(kalman, measure);		

	cvReleaseImage(&gray);
	cvReleaseMemStorage(&storage);  
}

int main(int argc, char **argv)
{
	CvCapture *video = 0;							//获取摄像头
	IplImage *frame = 0;								//当前摄像头的内容

	cascade = (CvHaarClassifierCascade*)cvLoad("haarcascade_frontalface_alt.xml", 0, 0, 0);        
	if(!cascade)  
	{
		cout<<"Haar特征分类器载入失败!"<<endl;
		Sleep(1500);
		return -1;
	}

	video = cvCreateCameraCapture(0);		//打开摄像头
	if (!video) 
	{
		cout<<"请先打开摄像头!"<<endl;
		cvReleaseHaarClassifierCascade(&cascade);
		Sleep(1500);
		return -1;
	}
	cvSetCaptureProperty(video,CV_CAP_PROP_FRAME_WIDTH , 320);
	cvSetCaptureProperty(video,CV_CAP_PROP_FRAME_HEIGHT, 240);

	//1.kalman filter setup
	kalman = cvCreateKalman( STATE_NUM, MEASURE_NUM, 0 );
	measure = cvCreateMat( MEASURE_NUM, 1, CV_32FC1 );
	float TransitionMatrix[STATE_NUM][STATE_NUM] = {		//transition matrix
		1, 0, 50, 0,
		0, 1, 0, 50,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	memcpy( kalman->transition_matrix->data.fl, TransitionMatrix, sizeof(TransitionMatrix));
	cvSetIdentity(kalman->measurement_matrix, cvRealScalar(1));
	cvSetIdentity(kalman->process_noise_cov, cvRealScalar(1e-5));
	cvSetIdentity(kalman->measurement_noise_cov, cvRealScalar(1e-1));
	cvSetIdentity(kalman->error_cov_post, cvRealScalar(1));
	CvRNG rng = cvRNG(-1);
	cvRandArr(&rng, kalman->state_post, CV_RAND_UNI, cvRealScalar(0), cvRealScalar(240));

	//outfile.open("faceDetectionAndTracking.txt");
	while(true)
	{
		frame = cvQueryFrame(video);
		if (frame == NULL)
		{
			cout<<"获取图像失败, 已关闭摄像头!"<<endl;
			break;
		}
		ZcFaceDetection(frame);
		cvShowImage("邹城_人脸跟踪_卡尔曼滤波", frame);

		if (cvWaitKey(3)==27)
			break;   
	}

	//outfile.close();
	cvReleaseCapture(&video);
	cvReleaseKalman(&kalman);
	cvReleaseMat(&measure);
	cvReleaseHaarClassifierCascade(&cascade);

	return 0;
}