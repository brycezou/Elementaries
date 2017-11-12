#include "obtainForegroundMask.h"
#include "FourierTransform.h"
#include "LocateHeadRegion.h"
#include "registerColor2Depth.h"
#include "FindContours.h"
#include "GetHeadRegionRect.h"
#include <fstream>
#include "svm.h"

using namespace std;


//在深度图像depthIpl和配准后的彩色图像regColorIpl上
//标记出人脸区域headRect
//说明：直接在原图上标记
void showHeadRectResult(IplImage *depthIpl, IplImage *regColorIpl, CvRect &headRect)
{
	cvRectangle(depthIpl, cvPoint(2*headRect.x, 2*headRect.y), cvPoint(2*(headRect.x+headRect.width), 2*(headRect.y+headRect.height)), CV_RGB(0,0,128), 2);
	cvShowImage("深度人头定位结果", depthIpl);
	cvRectangle(regColorIpl, cvPoint(2*headRect.x, 2*headRect.y), cvPoint(2*(headRect.x+headRect.width), 2*(headRect.y+headRect.height)), CV_RGB(255,0,0), 2);
	cvShowImage("彩色人头定位结果", regColorIpl);
}

//缩放输入图像src至新的尺寸
//缩放后的宽度为newWidth
//缩放后的高度为newHeight
//说明：会返回新的图像
IplImage* resizeInputImage(IplImage *src, int newWidth, int newHeight)
{
	IplImage *newSizeImg = cvCreateImage(cvSize(newWidth, newHeight), src->depth, src->nChannels);
	cvResize(src, newSizeImg, CV_INTER_LINEAR);
	return newSizeImg;
}

//在48x56的人脸区域上预定义的区域
CvRect EYES_RECT	= cvRect(6, 16, 36, 16);		//眼睛区域
CvRect NOSE_RECT	= cvRect(6, 24, 36, 16);		//鼻子区域
CvRect MOUTH_RECT	= cvRect(6, 32, 36, 16);	//嘴巴区域
CvRect CHIN_RECT = cvRect(6, 48, 36, 8);			//下巴区域

//高级人脸分块的特征向量
//彩色图像的彩色直方图, 彩色图像的灰度直方图
//彩色图像的Haar特征, 深度图像的Haar特征
int f_vc[4172];	

//得到图像color_img的彩色直方图(每个通道取16级灰度), 保存在f_vc数组的[0:4095]位
//和灰度直方图(64级灰度), 保存在f_vc数组的[4096:4159]位
void getColorAndGrayHistogram(IplImage *color_img)
{
	memset(f_vc, 0, sizeof(int)*4172);
	for (int i = 0; i < color_img->height; i++)
	{
		for (int j = 0; j < color_img->width; j++)
		{
				int rr = ((uchar*)(color_img->imageData+i*color_img->widthStep))[j*color_img->nChannels+0]; 
				int gg = ((uchar*)(color_img->imageData+i*color_img->widthStep))[j*color_img->nChannels+1]; 
				int bb = ((uchar*)(color_img->imageData+i*color_img->widthStep))[j*color_img->nChannels+2]; 
				f_vc[rr/16*256+gg/16*16+bb/16]++;							//统计彩色直方图 [0:4095]
				f_vc[((int)(0.30*rr+0.59*gg+0.11*bb))/4+4096]++;	//统计灰度直方图 [4096:4159]
		}
	}
}

struct svm_model* eyes_model;
struct svm_model* nose_model;
struct svm_model* mouth_model;
int EYES_PREDICT;
int NOSE_PREDICT;
int MOUTH_PREDICT;
int predictOcclusionByColor(IplImage *color_roi)
{
	//预测眼睛区域
	cvSetImageROI(color_roi, EYES_RECT);
	getColorAndGrayHistogram(color_roi);
	//svm节点(index:value)
	struct svm_node *eyes_node = (struct svm_node *) malloc(4173*sizeof(struct svm_node));
	for (int i = 0; i < 4172; i++) {
		eyes_node[i].index = i+1;
		eyes_node[i].value = f_vc[i];
	}
	eyes_node[4172].index = -1;
	int eyes_predict = (int)svm_predict(eyes_model, eyes_node);
	free(eyes_node);
	cvResetImageROI(color_roi);

	//预测鼻子区域
	cvSetImageROI(color_roi, NOSE_RECT);
	getColorAndGrayHistogram(color_roi);
	struct svm_node *nose_node = (struct svm_node *) malloc(4173*sizeof(struct svm_node));
	for (int i = 0; i < 4172; i++) {
		nose_node[i].index = i+1;
		nose_node[i].value = f_vc[i];
	}
	nose_node[4172].index = -1;
	int nose_predict = (int)svm_predict(nose_model, nose_node);
	free(nose_node);
	cvResetImageROI(color_roi);

	//预测嘴巴区域
	cvSetImageROI(color_roi, MOUTH_RECT);
	getColorAndGrayHistogram(color_roi);
	struct svm_node *mouth_node = (struct svm_node *) malloc(4173*sizeof(struct svm_node));
	for (int i = 0; i < 4172; i++) {
		mouth_node[i].index = i+1;
		mouth_node[i].value = f_vc[i];
	}
	mouth_node[4172].index = -1;
	int mouth_predict = (int)svm_predict(mouth_model, mouth_node);
	free(mouth_node);
	cvResetImageROI(color_roi);

	EYES_PREDICT = eyes_predict;
	NOSE_PREDICT = nose_predict;
	MOUTH_PREDICT = mouth_predict;

	cout<<eyes_predict<<", "<<nose_predict<<", "<<mouth_predict<<endl;
	return eyes_predict+nose_predict+mouth_predict;
}

int predictOcclution(IplImage *depthIpl, IplImage *regColorIpl, CvRect &headRect)
{
	//获得人脸感兴趣区域
	CvRect rect_roi = cvRect(2*headRect.x, 2*headRect.y, 2*headRect.width, 2*headRect.height);

	//将深度人脸图像缩放到48*56像素
	cvSetImageROI(depthIpl, rect_roi);
	IplImage *depth_roi = resizeInputImage(depthIpl, 48, 56);
	cvResetImageROI(depthIpl);

	//将彩色人脸图像缩放到48*56像素
	cvSetImageROI(regColorIpl, rect_roi);
	IplImage *color_roi = resizeInputImage(regColorIpl, 48, 56);
	cvResetImageROI(regColorIpl);

	int res = predictOcclusionByColor(color_roi);

	cvReleaseImage(&color_roi);
	cvReleaseImage(&depth_roi);

	return res;
}


int main(int argc, char **argv)
{
	if(argc != 3) {
		cout<<"only two argument \"[depth video] [color video]\" required"<<endl;	
		system("pause");	return -1;
	}

	//读取深度视频
	cout<<"loading depth video ..."<<endl;
	CvCapture *capDepth = cvCreateFileCapture(argv[1]);
	if (NULL == capDepth) {
		cout<<"depth video is invalid"<<endl;
		system("pause");	return -1;
	}
	cout<<"depth video loaded"<<endl;

	//读取彩色视频
	cout<<"loading color video ..."<<endl;
	CvCapture *capColor = cvCreateFileCapture(argv[2]);
	if (NULL == capColor) {
		cout<<"color video is invalid"<<endl;
		system("pause");	return -1;
	}
	cout<<"color video loaded"<<endl;

	//加载SVM模型文件
	cout<<"loading svm model ..."<<endl;
	eyes_model = svm_load_model("eyes.model");
	nose_model = svm_load_model("nose.model");
	mouth_model = svm_load_model("mouth.model");
	cout<<"svm model loaded"<<endl;

	//注意:保存的深度视频帧为3通道, 而算法中使用的是单通道
	IplImage *frame_depth_3c = cvQueryFrame(capDepth);
	IplImage *depthIpl = cvCreateImage(cvSize(640,480), 8, 1);
	IplImage *colorIpl = cvQueryFrame(capColor);
	IplImage *depth2show = NULL;
	IplImage *color2show = NULL;
	CvRect headRect;
	char sResultTxtOut[10];
	char sDetalResult[10];

	//将遮挡人脸检测结果写入视频文件
	//CvVideoWriter *writer = cvCreateVideoWriter("test.avi",CV_FOURCC('M','J','P','G'), 20, cvGetSize(colorIpl)); 
	
	while((frame_depth_3c != NULL) && (colorIpl != NULL))
	{
		//将三通道的深度视频帧转换成单通道的灰度图像
		cvCvtColor(frame_depth_3c, depthIpl, CV_BGR2GRAY);
		depth2show = cvCloneImage(depthIpl);
		IplImage *regColorIpl = GetHeadRegionRect(depthIpl, colorIpl, headRect, false);
		if (regColorIpl != NULL)
		{
			color2show = cvCloneImage(regColorIpl);
			int res = predictOcclution(depthIpl, regColorIpl, headRect);
			if(res >= 1)
			{
				cout<<"!!!!!!!!!!!!!!!!!!!!Occluded"<<endl;
				sprintf(sResultTxtOut, "Occluded");
			}
			else
			{
				cout<<"-----------------Safe"<<endl;
				sprintf(sResultTxtOut, "Safe");
			}

			CvFont font;
			cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, 1,1,0,2);
			sprintf(sDetalResult, "%d, %d, %d", EYES_PREDICT, NOSE_PREDICT, MOUTH_PREDICT);
			cvPutText(color2show,sDetalResult,cvPoint(10,50),&font,CV_RGB(255,0,0));
			cvPutText(color2show,sResultTxtOut,cvPoint(10,100),&font,CV_RGB(255,0,0));
			showHeadRectResult(depth2show, color2show, headRect);
			//cvWriteFrame(writer, color2show);  

			cvReleaseImage(&regColorIpl);
			cvReleaseImage(&color2show);
		}
		cvReleaseImage(&depth2show);
		if(cvWaitKey(1) == 27)
			break;
		frame_depth_3c = cvQueryFrame(capDepth);
		colorIpl = cvQueryFrame(capColor);
	}

	//cvReleaseVideoWriter(&writer);  

	cout<<"finished!"<<endl;
	//cvReleaseImage(&colorIpl);
	cvReleaseImage(&depthIpl);
	//cvReleaseImage(&frame_depth_3c);

	return 0;
}
