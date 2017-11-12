#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;


//���ݵ�Ӧ�Ծ���homography������ͼ��srcColor����У��
IplImage* RegisterColor2Depth(IplImage *srcColor, CvMat *homography)
{
	int width = srcColor->width;
	int height = srcColor->height;
	IplImage *tmpRgb = cvCreateImage(cvSize(width*2, height*2), srcColor->depth, srcColor->nChannels);
	cvWarpPerspective(srcColor, tmpRgb, homography);
	cvSetImageROI(tmpRgb, cvRect(0, 0, width, height));
	IplImage *dstColor = cvCloneImage(srcColor);
	cvCopy(tmpRgb, dstColor);
	cvResetImageROI(tmpRgb);
	cvReleaseImage(&tmpRgb);
	return dstColor;
}


int main(int argc, char **argv)
{
	//���ݵ�Լ��㵥Ӧ�Ծ���
	double src[] = { 284, 222, 205, 319, 225, 326, 327, 224, 402, 314, 389, 323, 358, 327,	//7
							  216, 175, 235, 230, 127, 399, 392, 390, 299, 231									//5
	};
	double dst[] = { 274, 220, 190, 336, 209, 342, 322, 223, 410, 328, 393, 335, 361, 351,
							  189, 168, 210, 230, 92,   418, 386, 406, 279, 233
	};
	CvMat src_points = cvMat( 12, 2, CV_64FC1, src );
	CvMat dst_points = cvMat( 12, 2, CV_64FC1, dst );
	CvMat *homography = cvCreateMat( 3, 3, CV_64FC1);
	cvFindHomography(&src_points, &dst_points, homography);
	 
	//�����Ӧ�Ծ���
	cout<<"��Ӧ�Ծ���:"<<endl;
	for (int i = 0; i < homography->rows; i++) {
		for (int j = 0; j < homography->cols; j++)
			cout<<homography->data.db[i*homography->cols+j]<<"\t\t";
		cout<<endl;
	}

	/*
		��Ӧ�Ծ���
		double warp[] = {	1.36788,				0.138139,			-112.907,		
										0.0304907,			1.43976,				-76.9039,		
										9.79889e-005,		0.000453478,		1					};
	*/

	//ʹ�õ�Ӧ�Ծ���У��ͼ��
	//������Ҳ����ֱ�Ӹ����õ�Ӧ�Ծ���
	IplImage *srcRgb = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
	IplImage *dstRgb = RegisterColor2Depth(srcRgb, homography);
	cvShowImage("У����ͼ��", dstRgb);

	cvWaitKey(0);
	cvReleaseImage(&srcRgb);
	cvReleaseImage(&dstRgb);
	return 0;
}