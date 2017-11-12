#include <iostream>
#include <fstream>
#include <vector>
#include <highgui.h>
#include <cv.h>
#include <Windows.h>

using namespace std;
using namespace cv;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////下面是在线选择特征的实现代码
#define GRAY_BIN_NUM	32
#define MAX_ITERARATION	5
#define TOP_NUM	10
#define DELTA_4_LOGLIKELIHOOD	0.001

//用于生成特征图像的系数结构
typedef struct FEATURE_FACTOR
{
	int pre_r;	//依次为RGB三个分量的系数
	int pre_g;
	int pre_b;
	int label;	//用于根据论文中的规则削减等价系数的标志位
}FFactor;

typedef struct VARIANCE_RATIO_IMAGE
{
	double variance_ratio;
	IplImage *feature_img;
	IplImage *weight_img;
}VImage;

//用于生成不同特征图像的所有系数矩阵, 共49个元素
vector<FFactor> gfFactorList;
vector<VImage> gTargetDataImgPairList;
vector<VImage> gCurrDataImgPairList;

//目标区域结构
typedef struct TargetRegion
{
	int x_center;		//中心点坐标
	int y_center;
	int half_width;	//半宽半高
	int half_height;
}TargetRegion;

//初始化系数矩阵
void ZcInitialFeatureFactorList()
{
	gfFactorList.clear();
	FFactor ff;
	for (int r = -2; r <= 2; r++)
	{
		for (int g = -2; g <= 2; g++)
		{
			for (int b = -2; b <= 2; b++)
			{
				if(r==0 && g==0 && b==0)
					continue;
				ff.pre_r = r;
				ff.pre_g = g;
				ff.pre_b = b;
				ff.label = 0;
				gfFactorList.push_back(ff);
			}
		}
	}
}

//比较两组系数是否等价
bool ZcompareTwoFF(FFactor ff1, FFactor ff2)
{
	if (ff1.pre_r == 0)
	{
		if (ff2.pre_r != 0)
			return false;
		if (ff1.pre_g == 0)
		{
			if (ff2.pre_g != 0)
				return false;
			return true;
		}
		else
		{
			if (ff2.pre_g == 0)
				return false;
			else
			{
				int factor;
				if(factor = ff1.pre_g/ff2.pre_g)
				{
					if (factor*ff2.pre_b==ff1.pre_b)
						return true;
					return false;
				}
				else
				{
					factor = ff2.pre_g/ff1.pre_g;
					if (factor*ff1.pre_b==ff2.pre_b)
						return true;
					return false;
				}
			}
		}
	}
	else
	{
		if (ff2.pre_r == 0)
			return false;
		else
		{
			int factor;
			if(factor = ff1.pre_r/ff2.pre_r)
			{
				if ((factor*ff2.pre_g==ff1.pre_g) && (factor*ff2.pre_b==ff1.pre_b) )
					return true;
				return false;
			}
			else
			{
				factor = ff2.pre_r/ff1.pre_r;
				if ((factor*ff1.pre_g==ff2.pre_g) && (factor*ff1.pre_b==ff2.pre_b) )
					return true;
				return false;
			}
		}
	}
}

//从所有系数中削减等价系数, 最终剩余49组有效系数
void ZcPruneFeatureFactorList()
{
	FFactor ff;
	for (unsigned int i = 0; i < gfFactorList.size(); i++)
	{
		ff = gfFactorList[i];
		if (ff.label == 1)
			continue;
		for (unsigned int j = i+1; j < gfFactorList.size(); j++)
		{
			if (gfFactorList[j].label == 1)
				continue;
			if (ZcompareTwoFF(ff, gfFactorList[j]))
				gfFactorList[j].label = 1;
		}
	}
	for (vector<FFactor>::iterator it = gfFactorList.begin(); it != gfFactorList.end(); )
	{
		if ((*it).label == 1)
			it = gfFactorList.erase(it);
		else
			it++;
	}
	for (unsigned int i = 0; i < gfFactorList.size(); i++)
	{
		cout<<i+1<<" : \t"<<gfFactorList[i].pre_r<<"\t"<<gfFactorList[i].pre_g<<"\t"<<gfFactorList[i].pre_b<<endl;
	}
}

//将特征"图像"new_int_img的所有"像素"规整到0-255之间
//width和height是图像的尺寸参数, min_value和max_value分别是new_int_img中的最小与最大值
IplImage* ZcNormalizeImage(int *new_int_img, int width, int height, int min_value, int max_value)
{
	IplImage *new_gray_img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int new_gray_value = (int)((float)(new_int_img[i*width+j]-min_value)/1.0/(max_value-min_value)*255);
			((uchar *)(new_gray_img->imageData+i*new_gray_img->widthStep))[j] = new_gray_value;
		}
	}
	return new_gray_img;
}

//计算图像img的特征图像, 选哪一种特征由fFactor决定
IplImage* ZcalcuOneFeatureImage(IplImage *img, FFactor &fFactor)
{
	CvScalar stemp;
	int max_value = -10000;
	int min_value = 10000;

	CvSize img_size = cvGetSize(img);
	int *new_int_img = new int[(img_size.height)*(img_size.width)];
	for (int i = 0; i < img_size.height; i++)
	{
		for (int j = 0; j < img_size.width; j++)
		{
			stemp = cvGet2D(img, i, j);
			int r = (int)stemp.val[2];
			int g = (int)stemp.val[1];
			int b = (int)stemp.val[0];
			int new_value = fFactor.pre_r*r+fFactor.pre_g*g+fFactor.pre_b*b;
			new_int_img[i*(img_size.width)+j] = new_value;
			if (new_value > max_value)	max_value = new_value;
			if(new_value < min_value)		min_value = new_value;
		}
	}
	//将特征"图像"的所有"像素"规整到0-255之间
	IplImage *new_gray_img = ZcNormalizeImage(new_int_img, img_size.width, img_size.height, min_value, max_value);
	delete []new_int_img;
	return new_gray_img;
}

//计算灰度图gray_img的GRAY_BIN_NUM级直方图
int* ZcalcuGrayImageHistogram(IplImage *gray_img)
{
	CvSize img_size = cvGetSize(gray_img);
	int *gray_hist = new int[GRAY_BIN_NUM];
	memset(gray_hist, 0, sizeof(int)*GRAY_BIN_NUM);
	for (int i = 0; i < img_size.height; i++)
	{
		for (int j = 0; j < img_size.width; j++)
		{
			int index = ((int)((uchar *)(gray_img->imageData+i*gray_img->widthStep))[j])/(256/GRAY_BIN_NUM);
			gray_hist[index]++;
		}
	}
	return gray_hist;
}

//计算对数似然比L的方差, 参数p为相应的概率分布
double ZcalcuLklihoodRatioVariance(double *L, double *p)
{
	//计算方法见论文3.3节
	double first_term = 0;
	double second_term = 0;
	for (int i = 0; i < GRAY_BIN_NUM; i++)
	{
		double pL = p[i]*L[i];
		first_term += pL*L[i];
		second_term += pL;
	}
	return (first_term-second_term*second_term);
}

//根据直方图反算权重图像
IplImage* ZcGetWeightImage(int *obj_bkg_label, IplImage *gray_img)
{
	CvSize img_size = cvGetSize(gray_img);
	IplImage *weight_img = cvCreateImage(img_size, IPL_DEPTH_8U, 1);
	for (int i = 0; i < img_size.height; i++)
	{
		for (int j = 0; j < img_size.width; j++)
		{
			int index = ((int)((uchar *)(gray_img->imageData+i*gray_img->widthStep))[j])/(256/GRAY_BIN_NUM);
			//if (obj_bkg_label[index] == 1)
			//	((uchar *)(weight_img->imageData+i*weight_img->widthStep))[j] = 255;
			//else
			//	((uchar *)(weight_img->imageData+i*weight_img->widthStep))[j] = 0;
			((uchar *)(weight_img->imageData+i*weight_img->widthStep))[j] = obj_bkg_label[index];
		}
	}
	return weight_img;
}

//参数gray_img是某一特征下的归一化灰度图, curr_tr是目标当前的位置
//该函数首先统计得到目标和背景的归一化直方图, 然后计算目标和背景的两类方差比和权重图像
VImage ZcGet2ClassVarianceRatioAndWeightImage(IplImage *gray_img, TargetRegion curr_tr)
{
	CvRect rect_obj, rect_bkg;
	//计算目标区域的位置
	rect_obj.x = curr_tr.x_center-curr_tr.half_width;
	rect_obj.y = curr_tr.y_center-curr_tr.half_height;
	rect_obj.width = 2*curr_tr.half_width+1;
	rect_obj.height = 2*curr_tr.half_height+1;
	if(rect_obj.x < 0)	rect_obj.x = 0;
	if(rect_obj.y < 0)	rect_obj.y = 0;
	//计算背景区域的位置
	int half_margin = MAX(curr_tr.half_height, curr_tr.half_width);
	int margin = (2*half_margin+1)*3/4;
	rect_bkg.x = curr_tr.x_center-curr_tr.half_width-margin;
	rect_bkg.y = curr_tr.y_center-curr_tr.half_height-margin;
	rect_bkg.width = 2*curr_tr.half_width+1+2*margin;
	rect_bkg.height = 2*curr_tr.half_height+1+2*margin;
	if(rect_bkg.x < 0)	rect_bkg.x = 0;
	if(rect_bkg.y < 0)	rect_bkg.y = 0;

	//计算目标区域的直方图
	cvSetImageROI(gray_img, rect_obj);
	IplImage *obj_img = cvCloneImage(gray_img);
	int *obj_hist = ZcalcuGrayImageHistogram(obj_img);
	//cvShowImage("obj", obj_img);
	cvReleaseImage(&obj_img);
	cvResetImageROI(gray_img);
	//计算背景区域的直方图
	cvSetImageROI(gray_img, rect_bkg);
	IplImage *bkg_img = cvCloneImage(gray_img);
	int *bkg_hist = ZcalcuGrayImageHistogram(bkg_img);
	//cvShowImage("bkg", bkg_img);
	cvReleaseImage(&bkg_img);
	cvResetImageROI(gray_img);
	for (int i = 0; i < GRAY_BIN_NUM; i++)
		bkg_hist[i] = bkg_hist[i]-obj_hist[i];
	//for (int i = 0; i < GRAY_BIN_NUM; i++)
	//{
	//	cout<<obj_hist[i]<<"\t"<<bkg_hist[i]<<endl;
	//}
	double sum_obj_hist = 0;
	double sum_bkg_hist = 0;
	for (int i = 0; i < GRAY_BIN_NUM; i++)
	{
		sum_obj_hist += obj_hist[i];
		sum_bkg_hist += bkg_hist[i];
	}
	//计算归一化直方图和对数似然比
	double *objHist_p = new double[GRAY_BIN_NUM];
	double *bkgHist_q = new double[GRAY_BIN_NUM];
	double *pq_mean = new double[GRAY_BIN_NUM];
	double *logLklihoodRatio = new double[GRAY_BIN_NUM];
	int *obj_bkg_label = new int[GRAY_BIN_NUM];		//obj:1, bkg:0;
	double log_max = -1000;
	double log_min = 1000;
	for (int i = 0; i < GRAY_BIN_NUM; i++)
	{
		objHist_p[i] = obj_hist[i]/1.0/sum_obj_hist;
		bkgHist_q[i] = bkg_hist[i]/1.0/sum_bkg_hist;
		pq_mean[i] = (objHist_p[i]+bkgHist_q[i])/2;
		logLklihoodRatio[i] = log((MAX(objHist_p[i], DELTA_4_LOGLIKELIHOOD))/(MAX(bkgHist_q[i], DELTA_4_LOGLIKELIHOOD)));
		if(log_max < logLklihoodRatio[i])	log_max = logLklihoodRatio[i];
		if(log_min > logLklihoodRatio[i])	log_min = logLklihoodRatio[i];
		//if (logLklihoodRatio[i] > 0 )
		//	obj_bkg_label[i] = 1;
		//else
		//	obj_bkg_label[i] = 0;
	}
	delete []obj_hist;
	delete []bkg_hist;
	//计算公式见论文第3.3节
	double Lp = ZcalcuLklihoodRatioVariance(logLklihoodRatio, objHist_p);
	double Lq = ZcalcuLklihoodRatioVariance(logLklihoodRatio, bkgHist_q);
	double Lpq = ZcalcuLklihoodRatioVariance(logLklihoodRatio, pq_mean);

	for (int i = 0; i < GRAY_BIN_NUM; i++ )
	{
		obj_bkg_label[i] = (int)((logLklihoodRatio[i]-log_min)/(log_max-log_min)*255);
	}

	IplImage *weight_img = ZcGetWeightImage(obj_bkg_label, gray_img);
	VImage dateImgPair;
	dateImgPair.variance_ratio = (Lpq/(MAX((Lp+Lq), DELTA_4_LOGLIKELIHOOD)));
	dateImgPair.feature_img = gray_img;
	dateImgPair.weight_img = weight_img;
	delete []objHist_p;
	delete []bkgHist_q;
	delete []logLklihoodRatio;
	delete []pq_mean;
	delete []obj_bkg_label;
	return dateImgPair;
}

//首先计算输入图像video_img在fFactor特征下的归一化灰度图, 然后计算目标和背景的两类方差比和权重图像
VImage ZcGet2ClassVarianceRatio(IplImage *video_img, FFactor &fFactor, TargetRegion curr_tr)
{
	IplImage *gray_img = ZcalcuOneFeatureImage(video_img, fFactor);
	VImage dataImgPair = ZcGet2ClassVarianceRatioAndWeightImage(gray_img, curr_tr);
	//cvShowImage("FeatureImage", dataImgPair.feature_img);
	//cvShowImage("WeightImage", dataImgPair.weight_img);
	return dataImgPair;
}

//自定义降序排序规则
bool ZcVarianceRatioSortRule(const VImage &leftData, const VImage &rightData)
{
	return leftData.variance_ratio > rightData.variance_ratio;
}

void ZcGetRankedTargetDataImageList(IplImage *video_img, TargetRegion &curr_tr)
{
	VImage dataImgPair;
	gTargetDataImgPairList.clear();
	for (unsigned int i = 0; i < gfFactorList.size(); i++)
	{
		dataImgPair = ZcGet2ClassVarianceRatio(video_img, gfFactorList[i], curr_tr);
		gTargetDataImgPairList.push_back(dataImgPair);
	}
	std::sort(gTargetDataImgPairList.begin(), gTargetDataImgPairList.end(), ZcVarianceRatioSortRule);
}

void ZcGetRankedObjectDataImageList(IplImage *video_img, TargetRegion curr_tr)
{
	VImage dataImgPair;
	for (unsigned int i = 0; i < gCurrDataImgPairList.size(); i++)
	{
		cvReleaseImage(&(gCurrDataImgPairList[i].feature_img));
		cvReleaseImage(&(gCurrDataImgPairList[i].weight_img));
	}
	gCurrDataImgPairList.clear();

	for (unsigned int i = 0; i < gfFactorList.size(); i++)
	{
		dataImgPair = ZcGet2ClassVarianceRatio(video_img, gfFactorList[i], curr_tr);
		gCurrDataImgPairList.push_back(dataImgPair);
	}
	std::sort(gCurrDataImgPairList.begin(), gCurrDataImgPairList.end(), ZcVarianceRatioSortRule);
}

CvRect gInitialRect;
bool gIsSelected = false;

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
void ZcalcuEpanechnikovKernel2(int width, int height, float *kernel_template)
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

//根据kernel_template计算图像img中tr所在区域的加权灰度直方图
void ZcalcuGrayHistogram(IplImage *gray_img, TargetRegion &tr, float *gray_hist, float *kernel_template)
{
	memset(gray_hist, 0, sizeof(float)*GRAY_BIN_NUM);
	CvSize img_size = cvGetSize(gray_img);
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
			int index = ((int)((uchar *)(gray_img->imageData+y*gray_img->widthStep))[x]) / (256/GRAY_BIN_NUM);
			gray_hist[index] += kernel_template[(y-y_up)*kernel_width+(x-x_left)];
		}
	}
}

//计算图像img中tr所在区域的核加权灰度直方图
void ZcalcuTargetGrayHistogram(IplImage *img, TargetRegion &tr, float *gray_hist)
{
	int width = 2*tr.half_width+1;
	int height = 2*tr.half_height+1;
	int target_size = width*height;
	float *kernel_template = new float[target_size];

	//计算目标模板的核估计
	ZcalcuEpanechnikovKernel2(width, height, kernel_template);
	//计算核加权灰度直方图
	ZcalcuGrayHistogram(img, tr, gray_hist, kernel_template);
	delete []kernel_template;
}

void ZcInitialMeanShiftTracker2(IplImage *initial_image, TargetRegion &tr)								
{
	ZcInitialFeatureFactorList();
	ZcPruneFeatureFactorList();
	ZcGetRankedTargetDataImageList(initial_image, tr);
	char filename[50];
	for (unsigned int i = 0; i < gfFactorList.size(); i++)
	{
		sprintf(filename, "%05d_featureImg.jpg\0", i+1);
		cvSaveImage(filename, (gTargetDataImgPairList[i].feature_img));
		sprintf(filename, "%05d_weightImg.jpg\0", i+1);
		cvSaveImage(filename, (gTargetDataImgPairList[i].weight_img));
	}
	cout<<"done"<<endl;
}

//计算两个直方图hist1和hist2之间的巴氏系数
float ZcalcuBhattacharyyaCoefficient2(float *hist1, float *hist2, int bin_num)
{
	float rho = 0;
	for (int i = 0; i < bin_num; i++)
		rho = rho + sqrt(hist1[i]*hist2[i]);
	return rho;
}

//Mean shift迭代算法
float ZcMeanShiftTracker2(IplImage *img, TargetRegion &tr, float *target_gray_hist)
{
	int x_center_old, y_center_old;
	int width = 2*tr.half_width+1;
	int height = 2*tr.half_height+1;
	int candidate_region_szie = width*height;
	float *kernel_template = new float[candidate_region_szie];
	float *weight = new float [candidate_region_szie];
	float *bin_ratio = new float [GRAY_BIN_NUM]; 
	float *candidate_gray_hist = new float[GRAY_BIN_NUM];
	float rho = 0;

	ZcalcuEpanechnikovKernel2(width, height,kernel_template);
	ZcalcuGrayHistogram(img, tr, candidate_gray_hist, kernel_template);
	for (int kk = 0; kk < MAX_ITERARATION; kk++)
	{
		memset(weight, 0, sizeof(float)*candidate_region_szie);
		for (int i = 0; i < GRAY_BIN_NUM; i++ )
		{
			bin_ratio[i] = (float) (candidate_gray_hist[i] > 1e-6 ? sqrt(target_gray_hist[i]/candidate_gray_hist[i]) : 0);
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
				int bin_index = ((int)((uchar *)(img->imageData+y*img->widthStep))[x]) / (256/GRAY_BIN_NUM);
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
			rho = ZcalcuBhattacharyyaCoefficient2(candidate_gray_hist, target_gray_hist, GRAY_BIN_NUM);
			break;
		}

		ZcalcuGrayHistogram(img, tr, candidate_gray_hist, kernel_template);
	}
	delete []candidate_gray_hist;
	delete []bin_ratio;
	delete []weight;
	delete []kernel_template;
	return rho;
}

IplImage* ZcGetFirstNTargetMeanImage(int N)
{
	CvSize img_size = cvGetSize(gTargetDataImgPairList[0].feature_img);
	float *mean_img = new float[img_size.width*img_size.height];
	memset(mean_img, 0, sizeof(float)*img_size.width*img_size.height);
	int M = MIN(N, (int)gTargetDataImgPairList.size());
	for (int i = 0; i < img_size.height; i++)
	{
		for (int j = 0; j < img_size.width; j++)
		{
			for (int k = 0; k < M; k++)
			{
				IplImage *temp_img = gTargetDataImgPairList[k].feature_img;
				int pixel_value = (int)(((uchar *)(temp_img->imageData+i*temp_img->widthStep))[j]);
				mean_img[i*img_size.width+j] += (float)pixel_value;
			}
		}
	}
	for (int i = 0; i < img_size.height; i++)
	{
		for (int j = 0; j < img_size.width; j++)
		{
			mean_img[i*img_size.width+j] /= M;
		}
	}
	IplImage *new_mean_img = cvCloneImage(gTargetDataImgPairList[0].feature_img);
	for (int i = 0; i < img_size.height; i++)
	{
		for (int j = 0; j < img_size.width; j++)
		{
			((uchar *)(new_mean_img->imageData+i*new_mean_img->widthStep))[j] = (uchar)mean_img[i*img_size.width+j];
		}
	}
	return new_mean_img;
}

IplImage* ZcGetFirstNCurrMeanImage(int N)
{
	CvSize img_size = cvGetSize(gCurrDataImgPairList[0].feature_img);
	float *mean_img = new float[img_size.width*img_size.height];
	memset(mean_img, 0, sizeof(float)*img_size.width*img_size.height);
	int M = MIN(N, (int)gCurrDataImgPairList.size());
	for (int i = 0; i < img_size.height; i++)
	{
		for (int j = 0; j < img_size.width; j++)
		{
			for (int k = 0; k < M; k++)
			{
				IplImage *temp_img = gCurrDataImgPairList[k].feature_img;
				int pixel_value = (int)(((uchar *)(temp_img->imageData+i*temp_img->widthStep))[j]);
				mean_img[i*img_size.width+j] += (float)pixel_value;
			}
		}
	}
	for (int i = 0; i < img_size.height; i++)
	{
		for (int j = 0; j < img_size.width; j++)
		{
			mean_img[i*img_size.width+j] /= M;
		}
	}
	IplImage *new_mean_img = cvCloneImage(gCurrDataImgPairList[0].feature_img);
	for (int i = 0; i < img_size.height; i++)
	{
		for (int j = 0; j < img_size.width; j++)
		{
			((uchar *)(new_mean_img->imageData+i*new_mean_img->widthStep))[j] = (uchar)mean_img[i*img_size.width+j];
		}
	}
	return new_mean_img;
}

int ZcGetMedianValue(int array[], int num)
{
	for(int i = 0; i <num-1; i++)
	{
		for(int j = 0; j < num-1-i; j++)
		{
			if(array[j] > array[j+1])
			{
				int temp = array[j];
				array[j] = array[j+1];
				array[j+1] = temp;
			}
		}
	}
	return array[num/2];
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
	ZcInitialMeanShiftTracker2(video_image, tr);
	float *target_gray_hist[TOP_NUM];
	TargetRegion trArray[TOP_NUM];
	for (int i = 0; i < TOP_NUM; i++)
	{
		trArray[i].x_center = tr.x_center;
		trArray[i].y_center =tr.y_center;
		trArray[i].half_width = tr.half_width;
		trArray[i].half_height = tr.half_height;
		target_gray_hist[i] = new float[GRAY_BIN_NUM];
		ZcalcuTargetGrayHistogram(gTargetDataImgPairList[i].weight_img, trArray[i], target_gray_hist[i]);
	}

	ofstream outfile("onlinemeanshift.txt");
	char filename[50];
	int index = 2;
	video_image = cvQueryFrame(video_captrue);
	while (video_image)
	{
		ZcGetRankedObjectDataImageList(video_image, tr);
		int sum_x = 0;
		int sum_y = 0;
		int sum_width = 0;
		int sum_height = 0;
		for (int i = 0; i < TOP_NUM; i++)
		{
			ZcMeanShiftTracker2(gCurrDataImgPairList[i].weight_img, trArray[i], target_gray_hist[i]);
		}
		int xarray[TOP_NUM];
		int yarray[TOP_NUM];
		for (int i = 0; i < TOP_NUM; i++)
		{
			xarray[i] = trArray[i].x_center;
			yarray[i] = trArray[i].y_center;
		}
		tr.x_center = ZcGetMedianValue(xarray, TOP_NUM);
		tr.y_center = ZcGetMedianValue(yarray, TOP_NUM);
		cvRectangle(video_image, cvPoint(tr.x_center-tr.half_width, tr.y_center-tr.half_height), cvPoint(tr.x_center+tr.half_width, tr.y_center+tr.half_height), cvScalar(0, 0, 255), 2);								
		cvShowImage("video_image", video_image);
		sprintf(filename, "1%04d.jpg", index);
		cvSaveImage(filename, video_image);
		outfile<<index<<"\t"<<tr.x_center<<"\t"<<tr.y_center<<endl;
		index++;
		for (int i = 0; i < TOP_NUM; i++)
		{
			trArray[i].x_center = tr.x_center;
			trArray[i].y_center =tr.y_center;
			trArray[i].half_width = tr.half_width;
			trArray[i].half_height = tr.half_height;
		}
		video_image = cvQueryFrame(video_captrue);
		cvWaitKey(1);
	}
	outfile.close();
	for (int i = 0; i < TOP_NUM; i++)
		delete [](target_gray_hist[i]);
	cvReleaseImage(&video_image);
	cvReleaseCapture(&video_captrue);

	cout<<"finished!"<<endl;
	cvWaitKey(0);
	cvDestroyAllWindows();
	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////下面是基本的mean shift实现代码(加权彩色直方图)
/*
CvRect gInitialRect;
bool gIsSelected = false;
#define EACH_BIN_NUM			8
#define COLOR_BIN_NUM		512
#define MAX_ITERARATION	20	

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
//Mean shift迭代算法
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
*/