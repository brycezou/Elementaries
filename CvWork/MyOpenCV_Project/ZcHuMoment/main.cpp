#include <highgui.h>
#include <cv.h>
#include <math.h>
#include <fstream>
#include <Windows.h>

using namespace std;
using namespace cv;

#define LENGTH		7
#define STDNUM		4					//标准模板的个数


typedef struct								//形状所占区域的边界
{
	int minx;
	int miny;
	int maxx;
	int maxy;
}ZcBorder;		

typedef struct								//标准模板文件的数据结构
{
	char label[8];								//类标签
	double* pHuMoment;					//Hu矩向量
}ZcSample;		

//初始化目标形状所在精确区域的边界
void ZcInitialBorder(ZcBorder& border)
{
	border.maxx = -1;
	border.maxy = -1;
	border.minx = 65535;
	border.miny = 65535;
}

//得到目标形状所在精确区域的边界
ZcBorder ZcGetBorder(Mat gray_mat)
{
	ZcBorder border;
	ZcInitialBorder(border);
	uchar* row_head = NULL;
	for (int i = 0; i < gray_mat.rows; i++)
	{
		row_head = gray_mat.ptr<uchar>(i);
		for (int j = 0; j < gray_mat.cols; j++)
		{
			if (row_head[j] == 0xff)
			{
				if (j < border.minx)		border.minx = j;
				if (j > border.maxx)	border.maxx = j;
				if (i < border.miny)		border.miny = i;
				if (i > border.maxy)	border.maxy = i;
			}
		}
	}
	return border;
}

//自己写的计算图像roi_mat的Hu矩的函数
double* ZcGetHuMoment(Mat roi_mat)
{
	double m[4][4] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	uchar* row_head = NULL;
	for (int i = 0; i < roi_mat.rows; i++)
	{
		row_head = roi_mat.ptr<uchar>(i);
		for (int j = 0; j < roi_mat.cols; j++)
		{
			if (row_head[j] == 0xff)
			{
				m[0][0] += 1;
				m[0][1] += j;
				m[0][2] += j*j;
				m[0][3] += j*j*j;
				m[1][0] += i;
				m[1][1] += i*j;
				m[1][2] += i*j*j;
				m[2][0] += i*i;
				m[2][1] += i*i*j;
				m[3][0] += i*i*i;
			}
		}
	}

	double x_ave = m[1][0]/1.0/m[0][0];
	double y_ave = m[0][1]/1.0/m[0][0];
	double u[4][4];
	u[0][0] = m[0][0];
	u[0][1] = 0;
	u[0][2] = m[0][2]-y_ave*m[0][1];
	u[0][3] = m[0][3]-3*y_ave*m[0][2]+2*y_ave*y_ave*m[0][1];
	u[1][0] = 0;
	u[1][1] = m[1][1]-y_ave*m[1][0];
	u[1][2] = m[1][2]-2*y_ave*m[1][1]-x_ave*m[0][2]+2*y_ave*y_ave*m[1][0];
	u[2][0] = m[2][0]-x_ave*m[1][0];
	u[2][1] = m[2][1]-2*x_ave*m[1][1]-y_ave*m[2][0]+2*x_ave*x_ave*m[0][1];
	u[3][0] = m[3][0]-3*x_ave*m[2][0]+2*x_ave*x_ave*m[1][0];

	double ita[4][4];
	for (int p = 0; p < 4; p++)
	{
		for (int q = 3-p; q >= 0; q--)
		{
			ita[p][q] = u[p][q]/1.0/pow(u[0][0], (p+q)/1.0/2+1);
		}
	}

	double *huMoment = new double[7];
	huMoment[0] = ita[2][0]+ita[0][2];
	huMoment[1] = pow(ita[2][0]+ita[0][2], 2)+4*pow(ita[1][1], 2);
	huMoment[2] = pow(ita[3][0]-3*ita[1][2], 2)+pow(3*ita[2][1]-ita[0][3], 2);
	huMoment[3] = pow(ita[3][0]+ita[1][2], 2)+pow(ita[2][1]+ita[0][3], 2);
	huMoment[4] = (ita[3][0]-3*ita[1][2])*(ita[3][0]+ita[1][2])*(pow(ita[3][0]+ita[1][2], 2)-3*pow(ita[2][1]+ita[0][3], 2))+
							(3*ita[2][1]-ita[0][3])*(ita[0][3]+ita[2][1])*(3*pow(ita[3][0]+ita[1][2], 2)-pow(ita[2][1]+ita[0][3], 2));
	huMoment[5] = (ita[2][0]-ita[0][2])*(pow(ita[3][0]+ita[1][2], 2)-pow(ita[2][1]+ita[0][3], 2))+4*ita[1][1]*(ita[3][0]+ita[1][2])*(ita[2][1]+ita[0][3]);
	huMoment[6] = (3*ita[2][1]-ita[0][3])*(ita[3][0]+ita[1][2])*(pow(ita[3][0]+ita[1][2], 2)-3*pow(ita[2][1]+ita[0][3], 2))+
							(3*ita[1][2]-ita[3][0])*(ita[2][1]+ita[0][3])*(3*pow(ita[3][0]+ita[1][2], 2)-pow(ita[2][1]+ita[0][3], 2));

	return huMoment;
}

//将Hu矩值写进文件里
void ZcWriteHuMoment2File(char* filename, double* huMoment)
{
	ofstream outfile("huMoment.txt", ios::app);
	char* label = strrchr(filename, '\\');					//从filename中提取出类别标签
	outfile<<++label<<"   "<<LENGTH<<endl;		//输出类标签label和Hu矩的长度LENGTH
	for (int i = 0; i < LENGTH; i++)
	{
		outfile<<huMoment[i]<<"  ";
	}
	outfile<<endl<<endl;
	outfile.close();
}

//以原始二值图像为命令行参数计算Hu矩值并写入到文件里
void ZcGetHuMoment(int argc, char** argv)
{
	for (int i = 0; i < argc-1; i++)
	{
		Mat gray_mat = imread(argv[i+1], CV_LOAD_IMAGE_GRAYSCALE);		//从argv[i+1]加载原始图像gray_mat
		imshow("original", gray_mat);																			//显示原始图像gray_mat
		ZcBorder border = ZcGetBorder(gray_mat);													//得到目标形状所在的精确区域border
		Mat roi_mat(gray_mat, Rect(border.minx, border.miny, border.maxx-border.minx+1, border.maxy-border.miny+1));	//得到精确区域图形
		imshow("ROI", roi_mat);																				//显示精确区域图像
		//imwrite("saved.jpg", roi_mat);
		double *huMoment = ZcGetHuMoment(roi_mat);											//计算roi_mat的Hu矩, 存储在huMoment中
		ZcWriteHuMoment2File(argv[i+1], huMoment);												//将Hu矩huMoment以追加方式写入文件, 从argv[i+1]中提取类别标签
		delete []huMoment;
		cvWaitKey(0);
		roi_mat.release();
		gray_mat.release();
		cvDestroyAllWindows();																				//善后工作	
	}
}

//从文件中加载4个用于分类的模板
ZcSample* ZcReadStdTemplate(int& factor_num)
{
	char sample_label[20];
	ZcSample* stdsample_array = new ZcSample[STDNUM];
	ifstream infile("stdard.txt");
	for (int i = 0; i < STDNUM; i++)
	{
		infile>>sample_label>>factor_num;
		sample_label[3] = '\0';
		strcpy(stdsample_array[i].label, sample_label);
		stdsample_array[i].pHuMoment = new double[factor_num];
		for (int j = 0; j < factor_num; j++)
		{
			infile>>(stdsample_array[i].pHuMoment)[j];
		}
	}
	infile.close();

	return stdsample_array;
}

//对一个样本one_sample进行分类测试, 正确返回1, 否则返回0
int ZcGiveLabel2OneSample(ZcSample* stdsample_array, ZcSample one_sample, int factor_num)
{
	double distances[STDNUM] = { 0, 0, 0, 0 };
	char* labels[STDNUM];
	double dbtemp = 0;
	for (int i = 0; i < STDNUM; i++)
	{
		labels[i] = new char[8];
		strcpy(labels[i], stdsample_array[i].label);
		for (int j = 0; j < factor_num; j++)
		{
			dbtemp = (stdsample_array[i].pHuMoment)[j]-one_sample.pHuMoment[j];			//1
			distances[i] += dbtemp*dbtemp;																			//1
			//dbtemp = 1-(one_sample.pHuMoment[j])/(stdsample_array[i].pHuMoment)[j];	//2
			//distances[i] += dbtemp*dbtemp;																		//2
			//dbtemp = (stdsample_array[i].pHuMoment)[j]-one_sample.pHuMoment[j];		//3
			//distances[i] += abs(dbtemp);																			//3
			//dbtemp = 1-(one_sample.pHuMoment[j])/(stdsample_array[i].pHuMoment)[j];	//4
			//distances[i] += abs(dbtemp);																			//4
		}
	}

	char labelmin[8];
	double dbmin = distances[0];
	strcpy(labelmin, labels[0]);
	for (int i = 1; i < STDNUM; i++)
	{
		if (distances[i] < dbmin)
		{
			dbmin = distances[i];
			strcpy(labelmin, labels[i]);
		}
	}

	for (int i = 0; i < STDNUM; i++)
	{
		delete []labels[i];
	}

	ofstream outfile("result.txt", ios::app);
	outfile<<one_sample.label<<'\t'<<labelmin<<endl;
	outfile.close();

	if (strcmp(one_sample.label, labelmin) == 0)
		return 1;
	else
		return 0;
}

//从文件filename中读入一系列样本, 进行分类测试, 返回识别率
double ZcCalculateDistance(ZcSample* stdsample_array, char* filename)
{
	int factor_num = 0;
	char sample_label[20];
	int sample_num = 0;
	int correct_num = 0;

	ifstream infile(filename);
	infile>>sample_label>>factor_num;
	while (factor_num != 0)
	{
		sample_label[3] = '\0';
		ZcSample one_sample;
		strcpy(one_sample.label, sample_label);
		one_sample.pHuMoment = new double[factor_num];
		for (int i = 0; i < factor_num; i++)
		{
			infile>>(one_sample.pHuMoment)[i];
		}
		correct_num += ZcGiveLabel2OneSample(stdsample_array, one_sample, factor_num);
		sample_num++;
		delete [](one_sample.pHuMoment);

		factor_num = 0;
		infile>>sample_label>>factor_num;
	}
	infile.close();

	return correct_num/1.0/sample_num*100;
}

//清除4个标准模板文件所占用的内存
void ZcReleaseStdArray(ZcSample* stdsample_array)
{
	for (int i = 0; i < STDNUM; i++)
	{
		delete [](stdsample_array[i].pHuMoment);
	}
	delete []stdsample_array;
}

void ZcGetFinalResult()
{
	int FACTOR_NUM = 0;
	ZcSample* stdsample_array = ZcReadStdTemplate(FACTOR_NUM);

	double easy_rate = ZcCalculateDistance(stdsample_array, "easy.txt");
	cout<<"Easy correct rate: "<<easy_rate<<"%"<<endl;
	double hard_rate = ZcCalculateDistance(stdsample_array, "hard.txt");
	cout<<"Hard correct rate: "<<hard_rate<<"%"<<endl;

	ofstream outfile("result.txt", ios::app);
	outfile<<endl<<"Easy correct rate: "<<easy_rate<<"%"<<endl;
	outfile<<endl<<"Hard correct rate: "<<hard_rate<<"%"<<endl;
	outfile.close();

	ZcReleaseStdArray(stdsample_array);
}

//使用OpenCV计算Hu矩, 与自己的结果进行比较
void ZcUseOpenCVHuMoment(int argc, char** argv)
{
	CvMoments M;
	CvHuMoments huM;
	double huMoment[LENGTH];
	for (int i = 0; i < argc-1; i++)
	{
		Mat gray_mat = imread(argv[i+1], CV_LOAD_IMAGE_GRAYSCALE);
		ZcBorder border = ZcGetBorder(gray_mat);
		Mat roi_mat(gray_mat, Rect(border.minx, border.miny, border.maxx-border.minx+1, border.maxy-border.miny+1));  
		IplImage roi_ipl = roi_mat;
		cvMoments(&roi_ipl, &M, 1);  
		cvGetHuMoments(&M, &huM);
		huMoment[0] = huM.hu1;
		huMoment[1] = huM.hu2;
		huMoment[2] = huM.hu3;
		huMoment[3] = huM.hu4;
		huMoment[4] = huM.hu5;
		huMoment[5] = huM.hu6;
		huMoment[6] = huM.hu7;
		ZcWriteHuMoment2File(argv[i+1], huMoment);
		roi_mat.release();
		gray_mat.release();
	}
}

int main(int argc, char** argv)
{	
	printf("Let\'s begin...\n");

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//如果有参数, 则如“bat0.bmp”的原始样本, 确定形状所在精确区域，计算Hu矩, 并以追加的方式保存到文件里
	//注意: 可以有多个参数
	//如果没有参数, 则程序所在目录下必须包含“easy.txt”, “hard.txt”和“stdard.txt”三个文件, 它们是包含类标签和Hu矩的文件
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (argc > 1)
		ZcGetHuMoment(argc, argv);						//自己写的Hu矩, 第7个分量和OpenCV差一个负号
		//ZcUseOpenCVHuMoment(argc, argv);		//使用OpenCV的Hu矩
	else
		ZcGetFinalResult();

	printf("Finished!");
	Sleep(1000);
	return 0;
}