#include <iostream>
#include <highgui.h>
#include <cv.h>
using namespace std;


#define STATE_NUM			4		//x, y, dx, dy
#define MEASURE_NUM	2		//x, y

CvPoint realPosition = cvPoint(400, 300);

//鼠标回调函数
void mouseEvent(int event, int x, int y, int flags, void *param )
{
	if (event==CV_EVENT_MOUSEMOVE) 
	{
		realPosition = cvPoint(x, y);
	}
}

int main (void)
{
	//1.kalman filter setup
	CvKalman* kalman = cvCreateKalman( STATE_NUM, MEASURE_NUM, 0 );
	CvMat* predictPosition = cvCreateMat( MEASURE_NUM, 1, CV_32FC1 );
	float TransitionMatrix[STATE_NUM][STATE_NUM] ={		//transition matrix
		1, 0, 1, 0,
		0, 1, 0, 1,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	memcpy( kalman->transition_matrix->data.fl, TransitionMatrix, sizeof(TransitionMatrix));
	cvSetIdentity(kalman->measurement_matrix, cvRealScalar(1));
	cvSetIdentity(kalman->process_noise_cov, cvRealScalar(1e-5));
	cvSetIdentity(kalman->measurement_noise_cov, cvRealScalar(1e-1));
	cvSetIdentity(kalman->error_cov_post, cvRealScalar(1));
	CvRNG rng = cvRNG(-1);
	cvRandArr(&rng, kalman->state_post, CV_RAND_UNI, cvRealScalar(0), cvRealScalar(600));

	cvNamedWindow("kalman"); 
	cvSetMouseCallback("kalman", mouseEvent);
	IplImage* img = cvCreateImage(cvSize(800, 600), 8, 3);
	const CvMat* prediction = 0;
	while (1)
	{
		//2.kalman prediction
		prediction = cvKalmanPredict(kalman, 0);
		CvPoint predict_pt = cvPoint((int)prediction->data.fl[0], (int)prediction->data.fl[1]);		//x, y
		//3.update predictPosition
		predictPosition->data.fl[0] = (float)realPosition.x;
		predictPosition->data.fl[1] = (float)realPosition.y;
		//4.update
		cvKalmanCorrect( kalman, predictPosition );		
		//draw 
		cvSet(img, cvScalar(255, 255, 255, 0));
		cvCircle(img, predict_pt, 5, CV_RGB(0, 255, 0), 3);					//predicted point with green
		cvCircle(img, realPosition, 5, CV_RGB(255, 0, 0), 3);				//current position with red

		cvShowImage("kalman", img);
		if (cvWaitKey(3)==27)
			break;   
	}      

	cvReleaseImage(&img);
	cvReleaseKalman(&kalman);
	return 0;
}
