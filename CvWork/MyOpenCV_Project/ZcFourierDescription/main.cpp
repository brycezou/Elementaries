#include <iostream>
#include <highgui.h>
#include <cv.h>
#include <fstream>
#include <Windows.h>

using namespace std;
using namespace cv;

#define PI					3.14159
#define LENGTH		30					//����Ҷ�����ӵĳ���
#define STDNUM		4					//��׼ģ��ĸ���

template<class T>
struct ZcPoint								//�����/���������ݽṹ
{
	T x;
	T y;
};					

typedef struct								//ͼ����ռ����ı߽�
{
	int minx;
	int miny;
	int maxx;
	int maxy;
}ZcBorder;		

typedef struct								//��׼ģ���ļ������ݽṹ
{
	char label[8];								//����ǩ
	double* pFourier;						//����Ҷ�任����
}ZcSample;		

//��ԭʼ�����ļ�filemane�ж���߽�����в�����
ZcPoint<int>* ZcReadDataFromFile(char* filemane, int& array_size, ZcBorder& border)
{
	border.minx = 65535;
	border.miny = 65535;
	border.maxx = -1;
	border.maxy = -1;		//���ڱ�����״������������������ĸ��߽�

	ifstream infile(filemane);
	infile>>array_size;		//������״�߽�����Ŀ													
	ZcPoint<int>* point_array = new ZcPoint<int>[array_size];		//��״�ı߽����������
	ZcPoint<int>* parray = point_array;
	for (int i = 0; i < array_size; i++)
	{
		infile>>parray->x;
		infile>>parray->y;
		if (parray->x < border.minx)	border.minx = parray->x;	
		if (parray->x > border.maxx)	border.maxx = parray->x;
		if (parray->y < border.miny)	border.miny = parray->y;
		if (parray->y > border.maxy)	border.maxy = parray->y;	//Ѱ����״������������������ĸ��߽�
		parray++;
	}
	infile.close();
	return point_array;
}

//������ı߽�����л��Ƶ�ͼ����
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

//����߽������point_array�ĸ���Ҷ��任������
ZcPoint<double>* ZcGetFourierFactor(ZcPoint<int>* point_array, int array_size)
{
	ZcPoint<double>* fourier_array = new ZcPoint<double>[array_size];	//����Ϊarray_size�ĸ���Ҷ�任��������
	memset(fourier_array, 0, array_size*sizeof(ZcPoint<double>));
	for(int u = 0; u < array_size-1; u++)	//����߽��ĸ���Ҷ�任
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

//���㸴����ģ��
double ZcGetComplexModulus(ZcPoint<double> complex_number)
{
	return sqrt(complex_number.x*complex_number.x+complex_number.y*complex_number.y);
}

//��һ������Ҷ������
void ZcNormalizeFourierFactor(ZcPoint<double>* fourier_array, int array_size)
{
	double z_one = ZcGetComplexModulus(fourier_array[1]);
	for (int i = 1; i < array_size-1; i++)
	{
		fourier_array[i].x = ZcGetComplexModulus(fourier_array[i])/z_one;
	}
}

//����һ����ĸ���Ҷ������fourier_arrayд�뵽�ļ���, ����ǩ��filename����ȡ
void ZcWriteFourier2File(char* filename, ZcPoint<double>* fourier_array, int array_size)
{
	ofstream outfile("normalized.txt", ios::app);
	char* label = strrchr(filename, '\\');					//���ļ�������ȡ������ǩ
	outfile<<++label<<"    "<<LENGTH<<endl;	//������ǩ�͸���Ҷ�����ӵ����г���д�뵽�ļ�
	for (int i = 1; i <= LENGTH; i++)
	{
		//outfile<<fourier_array[i].x<<"  "<<fourier_array[i].y<<"  ";
		outfile<<fourier_array[i].x<<"  ";
	}
	outfile<<endl<<endl;
	outfile.close();
}

//��ԭʼ�����ļ�Ϊ�����в��������һ���ĸ���Ҷ�����Ӳ�д�뵽�ļ���
void ZcGetAllFourierData(int argc, char** argv)
{
	int SIZE = 0;
	ZcBorder border;		//�洢��״�����������ĸ��߽�

	for (int i = 0; i < argc-1; i++)
	{
		ZcPoint<int>* point_array = ZcReadDataFromFile(argv[i+1], SIZE, border);	//���ļ�argv[i+1]����ԭʼ�߽�����е�point_array
		IplImage* ipl_border = ZcDrawBorder(point_array, SIZE, border);					//���ݱ߽������point_array���Ʊ߽��ͼ��ipl_border
		cvShowImage("show", ipl_border);																	//��ʾ�߽��ͼ��ipl_border
		//cvSaveImage("saved.jpg", ipl_border);
		ZcPoint<double>* fourier_array = ZcGetFourierFactor(point_array, SIZE-1);	//����߽������point_array�ĸ���Ҷ�任��fourier_array
		ZcNormalizeFourierFactor(fourier_array, SIZE-1);											//��һ������Ҷ������
		ZcWriteFourier2File(argv[i+1], fourier_array, SIZE-1);									//����һ���ĸ���Ҷ������д���ļ���, ����argv[i+1]�õ����ǩ
		delete []fourier_array;
		delete []point_array;
		cvReleaseImage(&ipl_border);																		//�ƺ���
		cvWaitKey(0);
	}
}

//���ļ��м���4�����ڷ����ģ��, ����Ҷ���������еĳ���Ϊfactor_num
ZcSample* ZcReadStdTemplate(int& factor_num)
{
	char sample_label[MAX_PATH];
	ZcSample* stdsample_array = new ZcSample[STDNUM];		//4����׼��״
	ifstream infile("stdard.txt");
	for (int i = 0; i < STDNUM; i++)
	{
		infile>>sample_label>>factor_num;	//�������ǩsample_label�͸���Ҷ���������еĳ���factor_num
		sample_label[3] = '\0';
		strcpy(stdsample_array[i].label, sample_label);
		stdsample_array[i].pFourier = new double[factor_num];
		for (int j = 0; j < factor_num; j++)
		{
			infile>>(stdsample_array[i].pFourier)[j];		//�����i����׼��״�ĸ���Ҷ�����ӵĵ�j������
		}
	}
	infile.close();

	return stdsample_array;
}

//��һ������one_sample���з������, ��ȷ����1, ���򷵻�0
int ZcGiveLabel2OneSample(ZcSample* stdsample_array, ZcSample one_sample, int factor_num)
{
	double distances[STDNUM] = { 0, 0, 0, 0 };	//�洢����one_sample��4����׼֮��ľ���
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
			dbmin = distances[i];				//�������С����Ϊͬ��
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

//���ļ�filename�ж���һϵ������, ���з������, ����ʶ����
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
		for (int i = 0; i < factor_num; i++)			//����һ������������������
		{
			infile>>(one_sample.pFourier)[i];
		}
		correct_num += ZcGiveLabel2OneSample(stdsample_array, one_sample, factor_num);	//������ȷ�ĸ���
		sample_num++;		//�����ܸ���
		delete [](one_sample.pFourier);

		factor_num = 0;
		infile>>sample_label>>factor_num;
	}
	infile.close();

	return correct_num/1.0/sample_num*100;
}

//���4����׼ģ���ļ���ռ�õ��ڴ�
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
	int FACTOR_NUM = 0;		//����Ҷ�����ӵ����г���
	ZcSample* stdsample_array = ZcReadStdTemplate(FACTOR_NUM);	//����4����׼������stdsample_array

	double easy_rate = ZcCalculateDistance(stdsample_array, "easy.txt");		//����easy�����ȷ��
	cout<<"Easy correct rate: "<<easy_rate<<"%"<<endl;
	double hard_rate = ZcCalculateDistance(stdsample_array, "hard.txt");		//����hard�����ȷ��
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
	//����в���, ����ظ�ʽ�硰bat0.txt����ԭʼ����, ���Ʊ߽��ͼ��, ���������Ϊ30�Ĺ�һ������Ҷ������, ����׷�ӵķ�ʽ���浽�ļ���
	//ע��: �����ж������
	//���û�в���, ���������Ŀ¼�±��������easy.txt��, ��hard.txt���͡�stdard.txt�������ļ�, �����ǰ������ǩ�͹�һ������Ҷ�����ӵ��ļ�
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (argc > 1)
		ZcGetAllFourierData(argc, argv);		
	else
		ZcGetFinalResult();


	printf("Finished!");
	Sleep(1000);
	return 0;
}