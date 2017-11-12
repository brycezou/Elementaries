#include <opencv2/opencv.hpp>


enum OTSU_METHOD
{
	ORG_OPT, ADV_OPT
};

//基于最大化类间方差的分割
//输入为待分割向量, 长度, 起始位置和结束位置
//输出为用于分割的阈值
int TwoClassOTSU(double *hist, int left, int right, int method)
{
	int maxIndex = left;
	double max_sigmaB = -1;

	for (int k = left+1; k < right-1; k++)
	{
		double p1=0, p2=0, m1=0, m2=0, mG=0;
		double s1=0, s2=0;
		double d1, d2, sigmaB;
		for (int i = left; i < right; i++)
		{
			if(i<=k)
			{
				p1 += hist[i];
				m1 += i*hist[i];
			}
			else
			{
				p2 += hist[i];
				m2 += i*hist[i];
			}
		}
		mG = m1+m2;
		m1 = m1/1.0/p1;	
		m2 = m2/1.0/p2;	
		d1 = m1-mG;
		d2 = m2-mG;
		for (int i = left; i <right; i++)
		{
			if(i<=k)
				s1 += (i-m1)*(i-m1)*hist[i];
			else
				s2 += (i-m2)*(i-m2)*hist[i];
		}

		switch(method)
		{
		case ORG_OPT:
			sigmaB = p1*d1*d1+p2*d2*d2; //original
			break;
		default:
			sigmaB = (p1*d1*d1+p2*d2*d2)/1.0/(s1/1.0/p1+s2/1.0/p2); //advanced 1
			break;
		}

		if (sigmaB > max_sigmaB)
		{
			max_sigmaB = sigmaB;
			maxIndex = k;
		}
	}
	return maxIndex;
}
