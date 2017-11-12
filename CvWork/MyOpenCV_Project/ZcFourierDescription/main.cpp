#include <iostream>
#include <highgui.h>
#include <cv.h>
#include <fstream>
#include <Windows.h>

using namespace std;
using namespace cv;

#define PI					3.14159
#define LENGTH		30					//傅里叶描述子的长度
#define STDNUM		4					//标准模板的个数

template<class T>
struct ZcPoint								//坐标点/复数的数据结构
{
	T x;
	T y;
};					

typedef struct								//图像所占区域的边界
{
	int minx;
	int miny;
	int maxx;
	int maxy;
}ZcBorder;		

typedef struct								//标准模板文件的数据结构
{
	char label[8];								//类别标签
	double* pFourier;						//傅里叶变换序列
}ZcSample;		

//从原始数据文件filemane中读入边界点序列并返回
ZcPoint<int>* ZcReadDataFromFile(char* filemane, int& array_size, ZcBorder& border)
{
	border.minx = 65535;
	border.miny = 65535;
	border.maxx = -1;
	border.maxy = -1;		//用于保存形状所在区域的左右上下四个边界

	ifstream infile(filemane);
	infile>>array_size;		//读入形状边界点的数目													
	ZcPoint<int>* point_array = new ZcPoint<int>[array_size];		//形状的边界点序列数组
	ZcPoint<int>* parray = point_array;
	for (int i = 0; i < array_size; i++)
	{
		infile>>parray->x;
		infile>>parray->y;
		if (parray->x < border.minx)	border.minx = parray->x;	
		if (parray->x > border.maxx)	border.maxx = parray->x;
		if (parray->y < border.miny)	border.miny = parray->y;
		if (parray->y > border.maxy)	border.maxy = parray->y;	//寻找形状所在区域的左右上下四个边界
		parray++;
	}
	infile.close();
	return point_array;
}

//将读入的边界点序列绘制到图像上
IplImage* ZcDrawBorder(ZcPoint<int>* point_array, int array_size, ZcBorder border)
{
	IplImage* ipl_border = cvCreateImage(cvSize(border.maxx-border.minx+1, border.maxy-border.miny+1), IPL_DEPTH_8U, 3);
	cvZero(ipl_border);
	for (int i = 0; i <array_size-1; i++)
	{
		cvDrawLine(ipl_border, cvPoint(point_array[i].x-border.minx, point_array[i].y-border.miny), cvPoint(point_array[i+1].x-border.minx, point_array[i+1].y-border.miny), cvScalar(0, 0, 255), 2); 
	}
	return ipl_border;
}

//计算边界点序列point_array的傅里叶描变换并返回
ZcPoint<double>* ZcGetFourierFactor(ZcPoint<int>* point_array, int array_size)
{
	ZcPoint<double>* fourier_array = new ZcPoint<double>[array_size];	//长度为array_size的傅里叶变换序列数组
	memset(fourier_array, 0, array_size*sizeof(ZcPoint<double>));
	for(int u = 0; u < array_size-1; u++)	//计算边界点的傅里叶变换
	{
		double sum_real = 0;
		double sum_image = 0;
		for (int k = 0; k < array_size; k++)
		{
			double theta = 2*PI*u*k/array_size;
			double sin_theta = sin(theta);
			double cos_theta = cos(theta);
			int x_k = point_array[k].x;
			int y_k = point_array[k].y;
			sum_real += x_k*cos_theta+y_k*sin_theta;
			sum_image += y_k*cos_theta-x_k*sin_theta;
		}
		fourier_array[u].x = sum_real/array_size;
		fourier_array[u].y = sum_image/array_size;
	}

	return fourier_array;
}

//计算复数的模长
double ZcGetComplexModulus(ZcPoint<double> complex_number)
{
	return sqrt(complex_number.x*complex_number.x+complex_number.y*complex_number.y);
}

//归一化傅里叶描述子
void ZcNormalizeFourierFactor(ZcPoint<double>* fourier_array, int array_size)
{
	double z_one = ZcGetComplexModulus(fourier_array[1]);
	for (int i = 1; i < array_size-1; i++)
	{
		fourier_array[i].x = ZcGetComplexModulus(fourier_array[i])/z_one;
	}
}

//将归一化后的傅里叶描述子fourier_array写入到文件里, 类别标签从filename中提取
void ZcWriteFourier2File(char* filename, ZcPoint<double>* fourier_array, int array_size)
{
	ofstream outfile("normalized.txt", ios::app);
	char* label = strrchr(filename, '\\');					//从文件名中提取出类别标签
	outfile<<++label<<"    "<<LENGTH<<endl;	//将类别标签和傅里叶描述子的序列长度写入到文件
	for (int i = 1; i <= LENGTH; i++)
	{
		//outfile<<fourier_array[i].x<<"  "<<fourier_array[i].y<<"  ";
		outfile<<fourier_array[i].x<<"  ";
	}
	outfile<<endl<<endl;
	outfile.close();
}

//以原始数据文件为命令行参数计算归一化的傅里叶描述子并写入到文件里
void ZcGetAllFourierData(int argc, char** argv)
{
	int SIZE = 0;
	ZcBorder border;		//存储形状的上下左右四个边界

	for (int i = 0; i < argc-1; i++)
	{
		ZcPoint<int>* point_array = ZcReadDataFromFile(argv[i+1], SIZE, border);	//从文件argv[i+1]读入原始边界点序列到point_array
		IplImage* ipl_border = ZcDrawBorder(point_array, SIZE, border);					//根据边界点序列point_array绘制边界点图像ipl_border
		cvShowImage("show", ipl_border);																	//显示边界点图像ipl_border
		//cvSaveImage("saved.jpg", ipl_border);
		ZcPoint<double>* fourier_array = ZcGetFourierFactor(point_array, SIZE-1);	//计算边界点序列point_array的傅里叶变换到fourier_array
		ZcNormalizeFourierFactor(fourier_array, SIZE-1);											//归一化傅里叶描述子
		ZcWriteFourier2File(argv[i+1], fourier_array, SIZE-1);									//将归一化的傅里叶描述子写到文件里, 根据argv[i+1]得到类标签
		delete []fourier_array;
		delete []point_array;
		cvReleaseImage(&ipl_border);																		//善后工作
		cvWaitKey(0);
	}
}

//从文件中加载4个用于分类的模板, 傅里叶描述子序列的长度为factor_num
ZcSample* ZcReadStdTemplate(int& factor_num)
{
	char sample_label[MAX_PATH];
	ZcSample* stdsample_array = new ZcSample[STDNUM];		//4个标准形状
	ifstream infile("stdard.txt");
	for (int i = 0; i < STDNUM; i++)
	{
		infile>>sample_label>>factor_num;	//读入类标签sample_label和傅里叶描述子序列的长度factor_num
		sample_label[3] = '\0';
		strcpy(stdsample_array[i].label, sample_label);
		stdsample_array[i].pFourier = new double[factor_num];
		for (int j = 0; j < factor_num; j++)
		{
			infile>>(stdsample_array[i].pFourier)[j];		//读入第i个标准形状的傅里叶描述子的第j个分量
		}
	}
	infile.close();

	return stdsample_array;
}

//对一个样本one_sample进行分类测试, 正确返回1, 否则返回0
int ZcGiveLabel2OneSample(ZcSample* stdsample_array, ZcSample one_sample, int factor_num)
{
	double distances[STDNUM] = { 0, 0, 0, 0 };	//存储样本one_sample和4个标准之间的距离
	char* labels[STDNUM];
	double dbtemp = 0;
	for (int i = 0; i < STDNUM; i++)
	{
		labels[i] = new char[8];
		strcpy(labels[i], stdsample_array[i].label);
		for (int j = 0; j < factor_num; j++)
		{
			//dbtemp = (stdsample_array[i].pFourier)[j]-one_sample.pFourier[j];			//1
			//distances[i] += dbtemp*dbtemp;															//1
			//dbtemp = 1-(one_sample.pFourier[j])/(stdsample_array[i].pFourier)[j];	//2
			//distances[i] += dbtemp*dbtemp;															//2
			dbtemp = (stdsample_array[i].pFourier)[j]-one_sample.pFourier[j];			//3
			distances[i] += abs(dbtemp);																	//3
			//dbtemp = 1-(one_sample.pFourier[j])/(stdsample_array[i].pFourier)[j];	//4
			//distances[i] += abs(dbtemp);																//4
		}
	}

	char labelmin[8];
	double dbmin = distances[0];
	strcpy(labelmin, labels[0]);
	for (int i = 1; i < STDNUM; i++)
	{
		if (distances[i] < dbmin)
		{
			dbmin = distances[i];				//与距离最小者判为同类
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
	char sample_label[MAX_PATH];
	int sample_num = 0;
	int correct_num = 0;

	ifstream infile(filename);
	infile>>sample_label>>factor_num;
	while (factor_num != 0)
	{
		sample_label[3] = '\0';
		ZcSample one_sample;
		strcpy(one_sample.label, sample_label);
		one_sample.pFourier = new double[factor_num];
		for (int i = 0; i < factor_num; i++)			//读入一个待分类样本的数据
		{
			infile>>(one_sample.pFourier)[i];
		}
		correct_num += ZcGiveLabel2OneSample(stdsample_array, one_sample, factor_num);	//分类正确的个数
		sample_num++;		//样本总个数
		delete [](one_sample.pFourier);

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
		delete [](stdsample_array[i].pFourier);
	}
	delete []stdsample_array;
}

void ZcGetFinalResult()
{
	int FACTOR_NUM = 0;		//傅里叶描述子的序列长度
	ZcSample* stdsample_array = ZcReadStdTemplate(FACTOR_NUM);	//读入4个标准样本到stdsample_array

	double easy_rate = ZcCalculateDistance(stdsample_array, "easy.txt");		//计算easy组的正确率
	cout<<"Easy correct rate: "<<easy_rate<<"%"<<endl;
	double hard_rate = ZcCalculateDistance(stdsample_array, "hard.txt");		//计算hard组的正确率
	cout<<"Hard correct rate: "<<hard_rate<<"%"<<endl;

	ofstream outfile("result.txt", ios::app);
	outfile<<endl<<"Easy correct rate: "<<easy_rate<<"%"<<endl;
	outfile<<endl<<"Hard correct rate: "<<hard_rate<<"%"<<endl;
	outfile.close();

	ZcReleaseStdArray(stdsample_array);
}

int main(int argc, char** argv)
{
	printf("Let\'s begin...\n");

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//如果有参数, 则加载格式如“bat0.txt”的原始样本, 绘制边界点图像, 计算其点数为30的归一化傅里叶描述子, 并以追加的方式保存到文件里
	//注意: 可以有多个参数
	//如果没有参数, 则程序所在目录下必须包含“easy.txt”, “hard.txt”和“stdard.txt”三个文件, 它们是包含类标签和归一化傅里叶描述子的文件
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (argc > 1)
		ZcGetAllFourierData(argc, argv);		
	else
		ZcGetFinalResult();


	printf("Finished!");
	Sleep(1000);
	return 0;
}