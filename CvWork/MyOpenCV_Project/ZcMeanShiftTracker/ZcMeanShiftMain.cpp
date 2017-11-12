#include <iostream>
#include <fstream>
#include <highgui.h>
#include <cv.h>
#include <Windows.h>

using namespace std;
using namespace cv;


CvRect gInitialRect;
bool gIsSelected = false;
#define EACH_BIN_NUM			8
#define COLOR_BIN_NUM		512
#define MAX_ITERARATION	20	

//目标区域结构
typedef struct TargetRegion
{
	int x_center;		//中心点坐标
	int y_center;
	int half_width;	//半宽半高
	int half_height;
}TargetRegion;

//手动选择目标的初始区域
void ZcMouseEvent( int event, int x, int y, int, void* )
{
	if(gIsSelected)	return;
	switch( event )
	{
	case CV_EVENT_LBUTTONDOWN:
		gInitialRect.x = x;
		gInitialRect.y = y;
		break;
	case CV_EVENT_LBUTTONUP:
		int width = x-gInitialRect.x;
		int height = y-gInitialRect.y;
		width = width+(width%2+1)%2;		//将初始目标区域的宽度和高度限定为奇数
		height = height+(height%2+1)%2;
		if (width > 4 && height > 4)
		{
			gInitialRect.width = width;
			gInitialRect.height = height;
			gIsSelected = true;
		}
		break;
	}
}

//计算宽度和高度分别为width和height的核函数模板kernel_template
void ZcalcuEpanechnikovKernel(int width, int height, float *kernel_template)
{
	int template_size = width*height;
	memset(kernel_template, 0, sizeof(float)*template_size);
	int x_center = width/2;
	int y_center = height/2;
	int max_radius_square = x_center*x_center+y_center*y_center;
	float normalize_factor = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			float radius_square = (float)(((y-y_center)*(y-y_center)+(x-x_center)*(x-x_center))*1.0/max_radius_square);
			float kernel_value = 1 - radius_square;
			kernel_template[y*width+x] = kernel_value;
			normalize_factor = normalize_factor + kernel_value;
		}
	}
	for ( int i = 0; i < template_size; i++ )
		kernel_template[i] = kernel_template[i]/normalize_factor;
}

//根据kernel_template计算图像img中tr所在区域的彩色直方图
void ZcalcuColorHistogram(IplImage *img, TargetRegion &tr, float *color_hist, float *kernel_template)
{
	memset(color_hist, 0, sizeof(float)*COLOR_BIN_NUM);
	CvSize img_size = cvGetSize(img);
	if (( tr.x_center < 0 ) || (tr.x_center >= img_size.width) || ( tr.y_center < 0 ) || ( tr.y_center >= img_size.height ))
		return;

	int x_left = tr.x_center-tr.half_width;
	int x_right = tr.x_center+tr.half_width;
	int y_up = tr.y_center-tr.half_height;
	int y_down = tr.y_center+tr.half_height;
	x_left = x_left <= 0 ? 0 : x_left;
	x_right = x_right >= img_size.width ? img_size.width : x_right;
	y_up = y_up <= 0 ? 0 : y_up;
	y_down = y_down >= img_size.height ? img_size.height : y_down;

	int kernel_width = 2*tr.half_width+1;
	for (int y = y_up; y < y_down; y++)
	{
		for (int x = x_left; x < x_right; x++)
		{
			CvScalar stemp = cvGet2D(img, y, x);
			int r = ((int)stemp.val[2]) / (256/EACH_BIN_NUM);
			int g = ((int)stemp.val[1]) / (256/EACH_BIN_NUM);
			int b = ((int)stemp.val[0]) / (256/EACH_BIN_NUM);
			int index = r*EACH_BIN_NUM*EACH_BIN_NUM + g*EACH_BIN_NUM + b;
			color_hist[index] += kernel_template[(y-y_up)*kernel_width+(x-x_left)];
			//color_hist[index] += 1;
		}
	}
}

//计算图像img中tr所在区域的核加权彩色直方图
void ZcalcuTargetHistogram(IplImage *img, TargetRegion &tr, float *color_hist)
{
	int width = 2*tr.half_width+1;
	int height = 2*tr.half_height+1;
	int target_size = width*height;
	float *kernel_template = new float[target_size];
	
	//计算目标模板的核估计
	ZcalcuEpanechnikovKernel(width, height, kernel_template);
	//计算核加权的彩色直方图
	ZcalcuColorHistogram(img, tr, color_hist, kernel_template);
	delete []kernel_template;
}

//根据初始图像initial_image和初始目标区域信息tr初始化mean shift跟踪器
float* ZcInitialMeanShiftTracker(IplImage *initial_image, TargetRegion &tr)								
{
	float *target_color_hist = new float[COLOR_BIN_NUM];
	 //计算目标区域直方图
	ZcalcuTargetHistogram(initial_image, tr, target_color_hist);
	return target_color_hist;
}

//计算两个直方图hist1和hist2之间的巴氏系数
float ZcalcuBhattacharyyaCoefficient(float *hist1, float *hist2, int bin_num)
{
	float rho = 0;
	for (int i = 0; i < bin_num; i++)
		rho = rho + sqrt(hist1[i]*hist2[i]);
	return rho;
}

float ZcMeanShiftTracker(IplImage *img, TargetRegion &tr, float *target_color_hist)
{
	int x_center_old, y_center_old;
	int width = 2*tr.half_width+1;
	int height = 2*tr.half_height+1;
	int candidate_region_szie = width*height;
	float *kernel_template = new float[candidate_region_szie];
	float *weight = new float [candidate_region_szie];
	float *bin_ratio = new float [COLOR_BIN_NUM]; 
	float *candidate_color_hist = new float[COLOR_BIN_NUM];
	float rho = 0;

	ZcalcuEpanechnikovKernel(width, height,kernel_template);
	ZcalcuColorHistogram(img, tr, candidate_color_hist, kernel_template);
	for (int kk = 0; kk < MAX_ITERARATION; kk++)
	{
		memset(weight, 0, sizeof(float)*candidate_region_szie);
		for (int i = 0; i < COLOR_BIN_NUM; i++ )
		{
			bin_ratio[i] = (float) (candidate_color_hist[i] > 1e-6 ? sqrt(target_color_hist[i]/candidate_color_hist[i]) : 0);
		}

		CvSize img_size = cvGetSize(img);
		int x_left = tr.x_center-tr.half_width;
		int x_right = tr.x_center+tr.half_width;
		int y_up = tr.y_center-tr.half_height;
		int y_down = tr.y_center+tr.half_height;
		x_left = x_left <= 0 ? 0 : x_left;
		x_right = x_right >= img_size.width ? img_size.width : x_right;
		y_up = y_up <= 0 ? 0 : y_up;
		y_down = y_down >= img_size.height ? img_size.height : y_down;

		int kernel_width = 2*tr.half_width+1;
		float sum_weight = 0;
		float x_center = 0;
		float y_center = 0;
		for (int y = y_up; y < y_down; y++)
		{
			for (int x = x_left; x < x_right; x++)
			{
				CvScalar stemp = cvGet2D(img, y, x);
				int r = ((int)stemp.val[2]) / (256/EACH_BIN_NUM);
				int g = ((int)stemp.val[1]) / (256/EACH_BIN_NUM);
				int b = ((int)stemp.val[0]) / (256/EACH_BIN_NUM);
				int bin_index = r*EACH_BIN_NUM*EACH_BIN_NUM + g*EACH_BIN_NUM + b;
				int weight_index = (y-y_up)*kernel_width+(x-x_left);
				weight[weight_index] = bin_ratio[bin_index];
				sum_weight += bin_ratio[bin_index];
				x_center += x*weight[weight_index];
				y_center += y*weight[weight_index];	
			}
		}
		x_center_old = tr.x_center;
		y_center_old = tr.y_center;
		tr.x_center = (int)(x_center / sum_weight + 0.5);
		tr.y_center = (int)(y_center / sum_weight + 0.5);
		float error = sqrt((float)((tr.x_center-x_center_old)*(tr.x_center-x_center_old)+(tr.y_center-y_center_old)*(tr.y_center-y_center_old)));
		if (error <= 1.0)
		{
			rho = ZcalcuBhattacharyyaCoefficient(candidate_color_hist, target_color_hist, COLOR_BIN_NUM);
			break;
		}

		ZcalcuColorHistogram(img, tr, candidate_color_hist, kernel_template);
	}
	delete []candidate_color_hist;
	delete []bin_ratio;
	delete []weight;
	delete []kernel_template;
	return rho;
}


int main(int argc, char** argv)
{
	if (argc < 2)
	{
		cout<<"Only one argument required! [video file]"<<endl;
		Sleep(3000);
		return -1;
	}
	cvNamedWindow("video_image");
	cvSetMouseCallback("video_image", ZcMouseEvent);
	CvCapture* video_captrue = cvCreateFileCapture(argv[1]);
	if (video_captrue == NULL)
	{
		cout<<"Video file is invalid!"<<endl;
		Sleep(2000);
		return -1;
	}
	IplImage* video_image = cvQueryFrame(video_captrue);
	if (video_image == NULL)
	{
		cout<<"Video image is invalid!"<<endl;
		cvReleaseCapture(&video_captrue);
		Sleep(2000);
		return -1;
	}
	cvShowImage("video_image", video_image);
	while (!gIsSelected)
	{
		cout<<"Please select the object to track, press any key to continue!"<<endl;
		cvWaitKey(0);
	}
	IplImage *img_first_show = cvCloneImage(video_image);
	cvRectangle(img_first_show, cvPoint(gInitialRect.x, gInitialRect.y), cvPoint(gInitialRect.x+gInitialRect.width, gInitialRect.y+gInitialRect.height), cvScalar(0, 0, 255), 2);								
	cvShowImage("video_image", img_first_show);
	cvReleaseImage(&img_first_show);
	cout<<"Press any key to continue!"<<endl;
	cvWaitKey(0);

	TargetRegion tr;
	tr.x_center = gInitialRect.x+gInitialRect.width/2;
	tr.y_center = gInitialRect.y+gInitialRect.height/2;
	tr.half_width = gInitialRect.width/2;
	tr.half_height = gInitialRect.height/2;
	float *target_color_hist = ZcInitialMeanShiftTracker(video_image, tr);

	ofstream outfile("meanshift.txt");
	char filename[50];
	int index = 2;
	video_image = cvQueryFrame(video_captrue);
	while (video_image)
	{
		ZcMeanShiftTracker(video_image, tr, target_color_hist);
		cvRectangle(video_image, cvPoint(tr.x_center-tr.half_width, tr.y_center-tr.half_height), cvPoint(tr.x_center+tr.half_width, tr.y_center+tr.half_height), cvScalar(0, 0, 255), 2);								
		cvShowImage("video_image", video_image);
		sprintf(filename, "%04d.jpg", index);
		cvSaveImage(filename, video_image);
		outfile<<index<<"\t"<<tr.x_center<<"\t"<<tr.y_center<<endl;
		index++;
		video_image = cvQueryFrame(video_captrue);
		cvWaitKey(25);
	}
	outfile.close();
	cvReleaseImage(&video_image);
	cvReleaseCapture(&video_captrue);

	cout<<"finished!"<<endl;
	cvWaitKey(0);
	cvDestroyAllWindows();
	return 0;
}