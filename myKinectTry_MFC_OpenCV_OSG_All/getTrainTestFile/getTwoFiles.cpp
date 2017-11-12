#include <iostream>
#include <fstream>
#include <time.h>
#include <Windows.h>
using namespace std;

//#define	NUM_EIGEN		16
int NUM_EIGEN;

typedef struct index_value
{
	int index;
	char comma;
	double value;
}IV;

//�����ݷ�Ϊѵ�����Ͳ��Լ�������
int main(int argc, char **argv)
{
	ifstream infile0;
	infile0.open("EigenNum.config");
	infile0>>NUM_EIGEN;
	infile0.close();

	char strFileName[20]={NULL};
	char strTrainName[20]={NULL};
	char strTestName[20]={NULL};
	char *pTemp=NULL;
	/***************************************************************/
	pTemp=strrchr(argv[1], '\\');
	strcat(strFileName, pTemp+1);
	ifstream infile;
	infile.open(strFileName);
	/***************************************************************/
	pTemp=strrchr(strFileName, '.');
	strncpy(strTrainName, strFileName, strlen(strFileName)-strlen(pTemp));
	strncpy(strTestName, strFileName, strlen(strFileName)-strlen(pTemp));
	strcat(strTrainName, ".trn");
	strcat(strTestName, ".tst");
	ofstream outrainfile, outestfile;
	outrainfile.open(strTrainName);
	outestfile.open(strTestName);
	/***************************************************************/
	cout<<strFileName<<endl<<strTrainName<<endl<<strTestName<<endl;
	/***************************************************************/

	int label;
	IV iv;
//	int line=0;
	int randint=-1;
	srand( time( NULL ) );//��ʱ��Ϊ�������� 

	while (!infile.eof())
	{
		randint = rand() % 100; //����0~99������� 
		//if (line%2==0)
		if (randint<50)
		{
			infile>>label;
			outrainfile<<label<<"  ";
			for (int i=0; i<NUM_EIGEN; i++)
			{
				infile>>iv.index>>iv.comma>>iv.value;
				if(i==NUM_EIGEN-1)
					outrainfile<<iv.index<<iv.comma<<iv.value<<endl;
				else
					outrainfile<<iv.index<<iv.comma<<iv.value<<"  ";
			}
		}
		else
		{
			infile>>label;
			outestfile<<label<<"  ";
			for (int i=0; i<NUM_EIGEN; i++)
			{
				infile>>iv.index>>iv.comma>>iv.value;
				if(i==NUM_EIGEN-1)
					outestfile<<iv.index<<iv.comma<<iv.value<<endl;
				else
					outestfile<<iv.index<<iv.comma<<iv.value<<"  ";
			}
		}
		//line++;
	}

	cout<<"finished!"<<endl;
	infile.close();
	outrainfile.close();
	outestfile.close();
	Sleep(1000);

	return 0;
}