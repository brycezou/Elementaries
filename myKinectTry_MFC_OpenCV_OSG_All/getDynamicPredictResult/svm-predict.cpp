#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <highgui.h>
#include <cv.h>
#include "svm.h"
using namespace std;

typedef struct ScaleNode
{
	int element;
	double minv;
	double maxv;
}SN;

struct svm_node *x;
const int max_nr_attr = 10;
struct svm_model* model;//训练得到的SVM模型
SN mySn[max_nr_attr];

double OnScaleValue(double fMin, double fMax, double minv, double maxv, double current)//规格化数据
{
	if(minv == maxv)
		return minv;

	if(current == minv)
		current = fMin;
	else if(current == maxv)
		current = fMax;
	else
		current = fMin + (fMax-fMin) * (current-minv)/(maxv-minv);

	return current;
}

double GetSVMRecogResult(IplImage * binary, double fMin, double fMax)
{
	CvMoments M;
	CvHuMoments huM;
	cvMoments(binary, &M, 1);  
	cvGetHuMoments(&M, &huM);
	x[0].value=huM.hu1;
	x[1].value=huM.hu2;
	x[2].value=huM.hu3;
	x[3].value=huM.hu4;
	x[4].value=huM.hu5;
	x[5].value=huM.hu6;
	x[6].value=huM.hu7;
	for (int i=0; i<7; i++)
	{
		x[i].index=i+1;
	}
	x[7].index=-1;
	cvReleaseImage(&binary);

	for (int k=0; k<7; k++)
	{
		x[k].value=OnScaleValue(fMin, fMax, mySn[k].minv, mySn[k].maxv, x[k].value);
	}

	double predict_label = svm_predict(model, x);//对一项测试数据进行预测，返回预测结果
	return predict_label;
}



int main(int argc, char **argv)
{
	IplImage * binary=0;
	ifstream infile;
	infile.open("1.range");
	char chX;
	double fMin, fMax;
	infile>>chX>>fMin>>fMax;
	for (int j=0; j<7; j++)
	{
		infile>>mySn[j].element>>mySn[j].minv>>mySn[j].maxv;
	}
	infile.close();

	model=svm_load_model("1.model");//model为训练得到的svm模型，该句加载svm模型
	x = (struct svm_node *) malloc(max_nr_attr*sizeof(struct svm_node));//x为svm节点（index：value）
	//从手势图像中直接获得SVM向量
	binary=cvLoadImage(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	double r=GetSVMRecogResult(binary, fMin, fMax);
	printf("%f\n", r);

	//cvMoments(binary, &M, 1);  
	//cvGetHuMoments(&M, &huM);
	//x[0].value=huM.hu1;
	//x[1].value=huM.hu2;
	//x[2].value=huM.hu3;
	//x[3].value=huM.hu4;
	//x[4].value=huM.hu5;
	//x[5].value=huM.hu6;
	//x[6].value=huM.hu7;
	//for (int i=0; i<7; i++)
	//{
	//	x[i].index=i+1;
	//}
	//x[7].index=-1;
	//cvReleaseImage(&binary);

	//for (int k=0; k<7; k++)
	//{
	//	x[k].value=OnScaleValue(fMin, fMax, mySn[k].minv, mySn[k].maxv, x[k].value);
	//}

	//double predict_label = svm_predict(model, x);//对一项测试数据进行预测，返回预测结果

	while (true)
	{
	}

	svm_free_and_destroy_model(&model);
	free(x);
	return 0;
}
