#include <fstream>
#include <highgui.h>
#include <cv.h>
using namespace std;

//得到手势二值图像的Hu矩
int main(int argc, char** argv)
{	
	IplImage * binary=0;
	CvMoments M;
	CvHuMoments huM;
	ofstream outfile;
	outfile.open("Hu矩.txt");
	if(!outfile)
	{
        printf("数据无法保存!");
	}
	char *strFileName;

	for (int i=0; i<argc-1; i++)
	{
		binary=cvLoadImage(argv[i+1], CV_LOAD_IMAGE_GRAYSCALE);
		cvMoments(binary, &M, 1);  
		cvGetHuMoments(&M, &huM);
		printf("%s\n%f  %f  %f  %f  %f  %f  %f\n", argv[i+1], huM.hu1, huM.hu2, huM.hu3, huM.hu4, huM.hu5, huM.hu6, huM.hu7);
		strFileName=strrchr(argv[i+1], '\\');
		outfile.precision(4);
		outfile<<strFileName[1]<<"  "<<"1:"<<huM.hu1<<"  2:"<<huM.hu2<<"  3:"<<huM.hu3<<"  4:"<<huM.hu4<<"  5:"<<huM.hu5<<"  6:"<<huM.hu6<<"  7:"<<huM.hu7<<endl;
	}
	
	outfile.close();
	cvWaitKey(0);
	cvReleaseImage(&binary);
	cvDestroyAllWindows();
	return 0;
}