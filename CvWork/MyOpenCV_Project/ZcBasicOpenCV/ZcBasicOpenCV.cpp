#include <highgui.h>
#include <cv.h>

using namespace std;

//����ɫͼ��תΪ�Ҷ�ͼ��, ���ı�ԭͼ 
IplImage* ZcRgb2Gray(IplImage *srcRgb)
{
	IplImage* dstGray = cvCreateImage(cvGetSize(srcRgb), IPL_DEPTH_8U, 1);
	cvCvtColor(srcRgb, dstGray, CV_BGR2GRAY); 
	return dstGray;
}

//������ͨ���Ҷ�ͼ�������ʾ��
//�̶���ֵ��ֵ��, ���ı�ԭͼ 
IplImage* ZcOperateGrayPixel(IplImage* srcGray)
{
	IplImage* dstBin = cvCreateImage(cvGetSize(srcGray), IPL_DEPTH_8U, 1);
	uchar pixel_value = 255;
	for (int i = 0; i < dstBin->height; i++)
	{
		for (int j = 0; j < dstBin->width; j++)
		{
			pixel_value = ((uchar *)(srcGray->imageData+i*srcGray->widthStep))[j];
			if (pixel_value > 150)
			{
				((uchar *)(dstBin->imageData+i*dstBin->widthStep))[j] = 255;
			}
			else
			{
				((uchar *)(dstBin->imageData+i*dstBin->widthStep))[j] = 0;			
			}
		}
	}
	return dstBin;
}

//������ͨ����ɫͼ�������ʾ��
//�滻�ض���ɫ��ֵ, ���ı�ԭͼ 
IplImage* ZcOperateRgbPixel(IplImage* srcRgb)
{
	IplImage* dstRgb = cvCreateImage(cvGetSize(srcRgb), IPL_DEPTH_8U, 3);
	CvScalar stemp = cvScalar(0, 0, 0); 
	for (int i = 0; i < srcRgb->height; i++)
	{
		for (int j = 0; j < srcRgb->width; j++)
		{
			stemp = cvGet2D(srcRgb, i, j);
			if (stemp.val[0] < 100 && stemp.val[1] < 100 && stemp.val[2] < 100)
			{
				cvSet2D(dstRgb, i, j, cvScalar(0, 0, 255));
			}
			else
			{
				cvSet2D(dstRgb, i, j, cvScalar(0, 255, 0));
			}
		}
	}
	return dstRgb;
}

//��Hough�任���ֱ��, ���ı�ԭͼ 
CvSeq* ZcGetHoughLines(IplImage* gray)
{
	CvMemStorage* storage = cvCreateMemStorage(0);
	IplImage* binary = cvCreateImage(cvGetSize(gray), IPL_DEPTH_8U, 1);
	cvCanny(gray, binary, 50, 200, 3);
	CvSeq* lines = cvHoughLines2(binary, storage, CV_HOUGH_STANDARD, 1, CV_PI/180, 100, 0, 0);
	cvReleaseImage(&binary);
	return lines;
}

//��Hough�任��⵽��ֱ�߻��Ƶ���ɫͼ�� 
void ZcDrawDetectedLines(IplImage* srcRgb, CvSeq* lines)
{
	CvPoint pt1 = cvPoint(0, 0), pt2 = cvPoint(0, 0);
	double cos_theta = 0, sin_theta = 0, x0 = 0, y0 = 0;
	float rho = 0, theta = 0;
	float* line = NULL;

	for(int i = 0; i < MIN(lines->total, 10); i++)
	{
		line = (float*)cvGetSeqElem(lines, i);
		rho = line[0];
		theta = line[1];
		cos_theta = cos(theta);
		sin_theta = sin(theta);
		x0 = cos_theta*rho;
		y0 = sin_theta*rho;
		pt1.x = cvRound(x0 + 1000*(-sin_theta));
		pt1.y = cvRound(y0 + 1000*(cos_theta));
		pt2.x = cvRound(x0 - 1000*(-sin_theta));
		pt2.y = cvRound(y0 - 1000*(cos_theta));
		cvLine(srcRgb, pt1, pt2, CV_RGB(255, 0, 0), 2, CV_AA);
	}
}

//��̬ѧ������, �ı�ԭͼ
void ZcMorphologyOpen(IplImage* srcBin, int iteration)
{
	IplImage* temp=cvCreateImage(cvGetSize(srcBin), IPL_DEPTH_8U, 1);
	cvMorphologyEx(srcBin, srcBin, temp, NULL, CV_MOP_OPEN, iteration);
	cvReleaseImage(&temp);
}

//��̬ѧ������, �ı�ԭͼ
void ZcMorphologyClose(IplImage* srcBin, int iteration)
{
	IplImage* temp=cvCreateImage(cvGetSize(srcBin), IPL_DEPTH_8U, 1);
	cvMorphologyEx(srcBin, srcBin, temp, NULL, CV_MOP_CLOSE, iteration);
	cvReleaseImage(&temp);
}

typedef struct  
{
	int minx;
	int maxx;
	int miny;
	int maxy;
}ZcBorder;
void ZcGetRotatedBound(int x, int y, double cos_value, double sin_value, ZcBorder& border)
{
	int rx = (int)floor(cos_value*x+sin_value*y);
	int ry = (int)floor(-sin_value*x+cos_value*y);
	if (rx < border.minx) border.minx = rx; 
	if (rx > border.maxx) border.maxx = rx;
	if (ry < border.miny) border.miny = ry; 
	if (ry > border.maxy) border.maxy = ry;
}

//��ʱ����תͼ��, ���ı�ԭͼ 
IplImage* ZcRotateImage(IplImage *srcImg, double angle)
{
	double angle_rad  = CV_PI*(angle/180);
	double sin_value = sin(angle_rad);
	double cos_value = cos(angle_rad);
	int height_new = (int)(fabs(sin_value*srcImg->width)+fabs(cos_value*srcImg->height));
	int width_new = (int)(fabs(sin_value*srcImg->height)+fabs(cos_value*srcImg->width));

	ZcBorder border; 
	border.minx = 0; border.miny = 0; border.maxx = 0; border.maxy = 0;
	ZcGetRotatedBound(srcImg->width-1, 0, cos_value, sin_value, border);
	ZcGetRotatedBound(0, srcImg->height-1, cos_value, sin_value, border);
	ZcGetRotatedBound(srcImg->width-1, srcImg->height-1, cos_value, sin_value, border);

	float m[6] = {(float)cos_value, (float)sin_value, (float)-border.minx, (float)-sin_value, (float)cos_value, (float)-border.miny};       
	CvMat M = cvMat(2, 3, CV_32F, m);

	IplImage* dstImg = cvCreateImage(cvSize(border.maxx-border.minx+1, border.maxy-border.miny+1), srcImg->depth, srcImg->nChannels);
	cvWarpAffine(srcImg, dstImg, &M, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS, cvScalarAll(0));

	return dstImg;
}

//�ڻҶ�ͼ���м������
CvSeq* ZcFaceDetection(IplImage* srcGray, CvHaarClassifierCascade* cascade)
{
	CvMemStorage* storage = cvCreateMemStorage(0);
	cvClearMemStorage(storage);
	cvEqualizeHist(srcGray, srcGray);
	CvSeq* faces = cvHaarDetectObjects(srcGray, cascade, storage, 1.2, 2, 0, cvSize(20, 20));
	return faces;
}

//����⵽���������Ƶ���ɫͼ��
void ZcDrawDetectedFaces(IplImage* srcRgb, CvSeq* faces)
{
	CvRect* rtemp = NULL;
	for( int i = 0; i < (faces ? faces->total : 0); i++ )
	{
		rtemp = (CvRect*)cvGetSeqElem(faces, i);
		cvRectangle(srcRgb, cvPoint(rtemp->x, rtemp->y), cvPoint(rtemp->x+rtemp->width, rtemp->y+rtemp->height), cvScalar(0, 0, 255), 1);	
	}
}

//��֡������Ƶ�ļ�, ���������������
void ZcPlayVideo(char* video_path, int delay)
{
	CvCapture* captrue = cvCreateFileCapture(video_path);
	IplImage* srcImg = cvQueryFrame(captrue);
	IplImage *dstImg = cvCreateImage(cvSize(500, (int)(srcImg->height*500/1.0/srcImg->width)), srcImg->depth, srcImg->nChannels);
	cvResize(srcImg, dstImg, CV_INTER_LINEAR);
	while (srcImg)
	{
		cvShowImage("08videoImg", dstImg);
		srcImg = cvQueryFrame(captrue);
		cvResize(srcImg, dstImg, CV_INTER_LINEAR);
		cvWaitKey(delay);
	}
	cvReleaseImage(&dstImg);
	cvReleaseImage(&srcImg);
	cvReleaseCapture(&captrue);
}


int main(int argc, char** argv)
{
	if(argc != 2)	return -1;

	//����ԭʼRGB��ɫͼ��
	IplImage* srcRgb = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
	cvShowImage("01srcRgb", srcRgb);
	//��ԭʼRGB��ɫͼ��ת��Ϊ�Ҷ�ͼ
	IplImage* srcGray = ZcRgb2Gray(srcRgb);
	cvShowImage("02srcGray", srcGray);

	//�ڻҶ�ͼ���м��ֱ�߲������ڲ�ɫͼ�ĸ�����
	CvSeq* lines = ZcGetHoughLines(srcGray);
	IplImage* dstRgb_HoughLine = cvCloneImage(srcRgb);
	ZcDrawDetectedLines(dstRgb_HoughLine, lines);
	cvReleaseMemStorage(&(lines->storage));  
	cvShowImage("03dstRgb_HoughLine", dstRgb_HoughLine);

	//�ڻҶ�ͼ���м�������������ڲ�ɫͼ�ĸ�����
	CvHaarClassifierCascade* cascade = (CvHaarClassifierCascade*)cvLoad("haarcascade_frontalface_alt.xml", 0, 0, 0 );
	CvSeq* faces = ZcFaceDetection(srcGray, cascade);
	IplImage* dstRgb_Faces = cvCloneImage(srcRgb);
	ZcDrawDetectedFaces(dstRgb_Faces, faces);
	cvReleaseMemStorage(&(faces->storage));  
	cvReleaseHaarClassifierCascade(&cascade);
	cvShowImage("04dstRgb_Faces", dstRgb_Faces);

	//�����ش���IplImage���͵�RGB��ɫͼ��
	IplImage* dstRgb_ChangePixel = ZcOperateRgbPixel(srcRgb);
	cvShowImage("05dstRgb_ChangePixel", dstRgb_ChangePixel);

	//�����ش���IplImage���͵ĻҶ�ͼ��
	IplImage* dstBin = ZcOperateGrayPixel(srcGray);
	cvShowImage("06dstBin", dstBin);

	//��ʱ�뽫ͼ����ת10��
	IplImage* dstRotate = ZcRotateImage(srcRgb, 10);
	cvShowImage("07dstRotate", dstRotate);

	//��֡������Ƶ�ļ�, ���������������
	ZcPlayVideo("videoplay.wmv", 20);


	cvWaitKey(0);
	cvReleaseImage(&dstRotate);
	cvReleaseImage(&dstBin);
	cvReleaseImage(&dstRgb_ChangePixel);
	cvReleaseImage(&dstRgb_Faces);
	cvReleaseImage(&dstRgb_HoughLine);
	cvReleaseImage(&srcGray);
	cvReleaseImage(&srcRgb);
	cvDestroyAllWindows();

	return 0;
}