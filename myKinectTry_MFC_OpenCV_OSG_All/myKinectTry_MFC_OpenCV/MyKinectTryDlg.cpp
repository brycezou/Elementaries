// MyKinectTryDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "myKinectTry_MFC_OpenCV.h"
#include "MyKinectTryDlg.h"
#include "afxdialogex.h"

#include "opencv2/opencv.hpp"
#include "CvvImage.h"
#include "DynaHandPos.h"
#include <fstream>
#include "svm.h"

using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum RECOGNIZE_RESULE 
{
	UP, DOWN, TOLEFT, TORIGHT, COMENEAR, GOFAR, COUNTERCLOCK, CLOCK, TOSTOP, NONE
};
int gRECOG_RESULT=NONE;
enum FIRST_CLASS_RESULT 
{
	F_CLOCK, F_NEARFAR, F_OTHER
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SEQ_NUM 10
#define STATIC_NUM 6
#define SAMPLE_NUM 17
#define RECOGNIZE_RATE 0.5
#define RECV_BUFFER_SIZE 512
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int gRSeqNum=0;
int gLSeqNum=0;
double gRE2HTheta=0;//right elbow to hand theta
double gLE2HTheta=0;//left elbow to hand theta
int gRGuessRate=0; 
int gLGuessRate=0; 
int gHipCenterY=0;
int gRHandX[SEQ_NUM]={-1,-1,-1,-1,-1, -1,-1, -1, -1, -1};
int gRElbowX[SEQ_NUM]={-1,-1,-1,-1,-1, -1,-1, -1, -1, -1};
int gLHandX[SEQ_NUM]={-1,-1,-1,-1,-1, -1,-1, -1, -1, -1};
int gLElbowX[SEQ_NUM]={-1,-1,-1,-1,-1, -1,-1, -1, -1, -1};

int gRUpDownSeqNum=0;
int gRUpGuessRate=0;
int gRDownGuessRate=0;
int gRHandY[SEQ_NUM]={-1,-1,-1,-1,-1, -1,-1, -1, -1, -1};
int gRElbowY[SEQ_NUM]={-1,-1,-1,-1,-1, -1,-1, -1, -1, -1};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CMyKinectTryDlg *pMyDlg;
char *path[]={"res\\0.jpg", "res\\1.jpg", "res\\2.jpg", "res\\3.jpg", "res\\4.jpg","res\\5.jpg","res\\6.jpg","res\\7.jpg"};
CvPoint skeletonPoint[NUI_SKELETON_COUNT][NUI_SKELETON_POSITION_COUNT]={cvPoint(0,0)}; 
void getDepthImage(HANDLE &depthEvent, HANDLE &depthStreamHandle, Mat &depthImage); 
void getSkeletonImage(HANDLE &skeletonEvent, Mat &skeletonImage); 
void drawSkeleton(Mat &image, CvPoint pointSet[], int witchone); 
void drawDynamicTrajectory(Mat &image);//draw the dynamic hand pos trajectory
void cvMySetMatEmpty(Mat &image);//clearn a specified Mat
double getAtan(CvPoint pt1, CvPoint pt2);
DWORD WINAPI OnListeningHand(LPVOID lpParam);
DWORD WINAPI OnProcessDepth(LPVOID lpParam);
DWORD WINAPI OnListingAccept(LPVOID lpParam);
DWORD WINAPI OnListeningCommand(LPVOID lpParam);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int gCurRhandX=0;
int gCurRhandY=0;//current right hand pos
int gCurLhandX=0;
int gCurLhandY=0;//current left hand pos
int gCurEyeY=0;
double gRH2Elength=0;//length between right hand and elbow
double gLH2Elength=0;//length between left hand and elbow
double gH2Lhlength=0;
CvScalar gCurColor=cvScalar(0, 0, 0); //current draw color
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool bDynamicStart=false;
bool bDynamicStop=false;//marks to mark where to start and stop
CDynaHandPos *pDhp;//hand pos info class
CPtrArray plist; //useful pos array
IplImage *ipl_SOS_label=0;//pic to load start and stop labels
IplImage *ipl_dynamicResult=0;
IplImage *ipl_phoneCall=0;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CMyKinectTryDlg::CMyKinectTryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyKinectTryDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hNextDepthFrameEvent=INVALID_HANDLE_VALUE;
}
CMyKinectTryDlg::~CMyKinectTryDlg()
{
    if (m_hNextDepthFrameEvent != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hNextDepthFrameEvent);
    }
}
void CMyKinectTryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
void CMyKinectTryDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
HCURSOR CMyKinectTryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
BEGIN_MESSAGE_MAP(CMyKinectTryDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL CMyKinectTryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	pMyDlg=this;

	if(OnInitKinect())
	{
		m_depthImage.create(240, 320, CV_8UC3); 
		m_skeletonImage.create(240, 320, CV_8UC3); 
		m_trajectoryImage.create(240, 320, CV_8UC3); 
		m_sampledImage.create(240, 320, CV_8UC3); 
		cvMySetMatEmpty(m_depthImage);
		cvMySetMatEmpty(m_skeletonImage);
		cvMySetMatEmpty(m_trajectoryImage);
		cvMySetMatEmpty(m_sampledImage);

		m_hNextDepthFrameEvent = CreateEvent( NULL, TRUE, FALSE, NULL ); // 定义事件句柄，创建读取下一帧的信号事件句柄，控制KINECT是否可以开始读取下一帧的深度数据
		m_skeletonEvent = CreateEvent( NULL, TRUE, FALSE, NULL ); 
		m_pDepthStreamHandle=INVALID_HANDLE_VALUE;// 保存深度图像数据流的句柄，用以提取数据

		// NuiImageStreamOpen 打开Kinect的数据流 
		// 参数依次为：数据流类型，图像分辨率，无用参数NULL，建立的缓冲个数这里取了2，控制KINECT是否可以开始读取下一帧的句柄，保存数据的句柄即输出
		// 打开带ID的深度数据流
		HRESULT hr = NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX, NUI_IMAGE_RESOLUTION_320x240, NULL, 2, this->m_hNextDepthFrameEvent, &this->m_pDepthStreamHandle); 
		if( hr != S_OK )   
		{   
			NuiShutdown(); 
			if (m_hNextDepthFrameEvent != INVALID_HANDLE_VALUE)
			{
				CloseHandle(m_hNextDepthFrameEvent);
			}
			if (m_pDepthStreamHandle != INVALID_HANDLE_VALUE)
			{
				CloseHandle(m_pDepthStreamHandle);
			}
			free(&m_depthImage);
			MessageBox(_T("图像数据流打开失败!"));
		}
		hr = NuiSkeletonTrackingEnable(this->m_skeletonEvent, 0 ); 
		if( hr != S_OK )   
		{   
			NuiShutdown(); 
			if (m_skeletonEvent != INVALID_HANDLE_VALUE)
			{
				CloseHandle(m_skeletonEvent);
			}
			free(&m_skeletonImage);
			free(&m_trajectoryImage);
			free(&m_sampledImage);
			MessageBox(_T("深度数据流打开失败!"));
		} 

		for(int i=0; i<SEQ_NUM; i++)
		{
			gRHandX[i]=-1;
			gRElbowX[i]=-1;
		}

		HANDLE handle=CreateThread(NULL, 0, OnProcessDepth, NULL, 0, 0);
		if(!handle)
			MessageBox(_T("创建深度图像处理线程失败!"));

		CloseHandle(handle);

		HANDLE handle2=CreateThread(NULL, 0, OnListeningHand, NULL, 0, 0);
		if(!handle2)
			MessageBox(_T("创建骨骼数据处理线程失败!"));

		CloseHandle(handle2);

	}

	m_OSG = new CCoreOSG(GetDlgItem(IDC_FRAME_OSG)->m_hWnd);
	m_OSG->InitOSG();
	m_OSGThreadHandle = (HANDLE)_beginthread(&CCoreOSG::Render, 0, m_OSG) ;

	OnOpenRecvSocket(51112);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
BOOL CMyKinectTryDlg::OnInitKinect(void)
{
	HRESULT hr = NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_SKELETON); // 调用NUI初始化函数
    if( hr != S_OK )   
    {   
		MessageBox(_T("设备初始化失败!"));
		return FALSE;
    } 
	return TRUE;
}
void CMyKinectTryDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	NuiShutdown();
	CDialogEx::OnClose();
}
void CMyKinectTryDlg::OnShowImage(IplImage* img, UINT ID)
{
	CDC* pDC = GetDlgItem(ID)->GetDC();   // 获得显示控件的DC
    HDC hdc = pDC->GetSafeHdc(); // 获取 HDC(设备句柄) 来进行绘图操作
    CRect rect;
    GetDlgItem(ID)->GetClientRect( &rect );
    CvvImage cimg;
    cimg.CopyOf( img );                // 复制图片
    cimg.DrawToHDC( hdc, &rect );  // 将图片绘制到显示控件的指定区域内
    ReleaseDC( pDC );
	cimg.Destroy();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////something to use libSVM
typedef struct ScaleNode //加载规格化文件需要的自定义节点
{
	int element;
	double minv;
	double maxv;
}SN;
struct svm_node *svmX_StaticHand;//libSVM定义的的节点
struct svm_node *svmX_DynamicHand;
const int max_nr_attr_static_hand = 10;
const int max_nr_attr_dynamic_hand = 20;
struct svm_model* static_model;//训练得到的SVM模型
struct svm_model* clock_model;
struct svm_model* nearfar_model;
SN mySn_static_hand[max_nr_attr_static_hand];//保存规格化文件中的数据
SN mySn_dynamic_clock[max_nr_attr_dynamic_hand];
SN mySn_dynamic_nearfar[max_nr_attr_dynamic_hand];
//规格化数据
double OnScaleValue(double fMin, double fMax, double minv, double maxv, double current)
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
//得到当前二值手势的识别结果
double GetStaticHandSVMRecogResult(IplImage * binary, double fMin, double fMax)
{
	CvMoments M;
	CvHuMoments huM;
	cvMoments(binary, &M, 1);  
	cvGetHuMoments(&M, &huM);
	svmX_StaticHand[0].value=huM.hu1;
	svmX_StaticHand[1].value=huM.hu2;
	svmX_StaticHand[2].value=huM.hu3;
	svmX_StaticHand[3].value=huM.hu4;
	svmX_StaticHand[4].value=huM.hu5;
	svmX_StaticHand[5].value=huM.hu6;
	svmX_StaticHand[6].value=huM.hu7;
	for (int i=0; i<7; i++)
	{
		svmX_StaticHand[i].index=i+1;
	}
	svmX_StaticHand[7].index=-1;

	for (int k=0; k<7; k++)
	{
		svmX_StaticHand[k].value=OnScaleValue(fMin, fMax, mySn_static_hand[k].minv, mySn_static_hand[k].maxv, svmX_StaticHand[k].value);
	}

	double predict_label = svm_predict(static_model, svmX_StaticHand);//对一项测试数据进行预测，返回预测结果
	return predict_label;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//从Kinect获取深度图像（带人物ID）
void getDepthImage(HANDLE &depthEvent, HANDLE &depthStreamHandle, Mat &depthImage) 
{ 
    const NUI_IMAGE_FRAME *depthFrame = NULL; // 深度图像帧

    NuiImageStreamGetNextFrame(depthStreamHandle, 0, &depthFrame); // 从打开的深度数据流句柄depthStreamHandle中得到数据，保存于depthFrame的NUI_IMAGE_FRAME中
    INuiFrameTexture *depthTexture = depthFrame->pFrameTexture;   // INuiFrameTexture接口，容纳图像数据的对象
	NUI_LOCKED_RECT depthLockedRect;  // 提取数据帧到depthLockedRect，并锁定数据
    depthTexture->LockRect(0, &depthLockedRect, NULL, 0);   

    RGBQUAD pixeldata; 	// RGBQUAD是一个结构体，保存一个像素点的RGB值

	// 至此，LockedRect是真正保存Kinect数据的对象，pitch为每行字节数，pBits为第一个字节地址
    if( depthLockedRect.Pitch != 0 ) 
    { 
        for (int i=0; i<depthImage.rows; i++) 
        { 
            uchar *depthhead = depthImage.ptr<uchar>(i);   // 第 i 行的指针，一个字节uchar代表一个颜色信息
            uchar *depthBuffer = (uchar*)(depthLockedRect.pBits) + i * depthLockedRect.Pitch;
			USHORT *depthBufferRun = (USHORT*) depthBuffer; 			// 带ID深度图中每个像素由两字节16位表示，低3位是人物ID，高13位是距离信息
            for (int j=0; j<depthImage.cols; j++) 
            { 
                int personID = depthBufferRun[j]&7;  // 提取低3位人物ID
                int depthData = (depthBufferRun[j]&0xfff8) >> 3; // 提取高13位距离信息
                uchar GZdepthData = 255-(uchar)(256*depthData/0x0fff);  
                pixeldata.rgbBlue = pixeldata.rgbGreen = pixeldata.rgbRed = 0; 
                switch(personID) 
                { 
					case 0:  pixeldata.rgbRed = GZdepthData / 2;   pixeldata.rgbBlue = GZdepthData / 2;   pixeldata.rgbGreen =GZdepthData / 2;   break;    // 背景
					case 1:  pixeldata.rgbRed = 255;	break;   
					case 2:  pixeldata.rgbGreen = 255;	break;   
					case 3:  pixeldata.rgbBlue = 255;	break;   
					case 4:  pixeldata.rgbRed = 255;	pixeldata.rgbGreen =255;   break;   
					case 5:  pixeldata.rgbRed = 255;	pixeldata.rgbBlue = 255;	break;   
					case 6:  pixeldata.rgbBlue = 255;	pixeldata.rgbGreen = 255;	break;   
					default:	break;
				} 	 
				depthhead[3*j] = pixeldata.rgbRed; 
				depthhead[3*j+1] = pixeldata.rgbGreen; 
				depthhead[3*j+2] = pixeldata.rgbBlue; 
            } 
        } 
    } 
    else 
    { 
        MessageBox(NULL, (_T("获取深度数据失败!")), (_T("错误 ")), 0);
    } 
	
	depthTexture->UnlockRect(0); 	// 本帧数据已处理完故解锁
    NuiImageStreamReleaseFrame(depthStreamHandle, depthFrame);  	// 释放本帧数据，准备迎接下一帧
} 
// 从Kinect获取人物骨架
void getSkeletonImage(HANDLE &skeletonEvent, Mat &skeletonImage) 
{ 
    NUI_SKELETON_FRAME skeletonFrame = {0}; 
    bool bFoundSkeleton = false; 
 
	// 从刚才打开数据流句柄中得到该帧数据，存于skeletonFrame
    if( NuiSkeletonGetNextFrame( 0, &skeletonFrame ) == S_OK )   
    {   
        for( int i = 0 ; i < NUI_SKELETON_COUNT ; i++ )   
        {   
            if( skeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED ) 
            {   
                bFoundSkeleton = true;   
                break; 
            }   
        }   
    } 
    else 
    { 
        MessageBox(NULL, (_T("获取骨骼数据失败!")), (_T("错误 ")), 0);
        return;  
    } 
 
    if( !bFoundSkeleton )   
    {   
        return;  
    }   
 
    // 平滑骨骼帧,消除抖动
	NuiTransformSmooth(&skeletonFrame, NULL);   
    skeletonImage.setTo(0);   
	
    for( int i = 0 ; i < NUI_SKELETON_COUNT ; i++ )   
    {   
        // 断定是否是一个正确骨骼的条件：骨骼被跟踪到并且肩部中心即颈部位置必须被跟踪到
		if( skeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED &&   
            skeletonFrame.SkeletonData[i].eSkeletonPositionTrackingState[NUI_SKELETON_POSITION_SHOULDER_CENTER] != NUI_SKELETON_POSITION_NOT_TRACKED)   
        {   
            float fx, fy;   

            // NuiTransformSkeletonToDepthImage将骨骼图像中的关节点坐标转化到深度图像中
			for ( int j = 0; j < NUI_SKELETON_POSITION_COUNT; j++ )   
            {   
                NuiTransformSkeletonToDepthImage(skeletonFrame.SkeletonData[i].SkeletonPositions[j], &fx, &fy );   
                skeletonPoint[i][j].x = (int)fx;   
                skeletonPoint[i][j].y = (int)fy; // 深度图像中的关节点坐标
				circle(skeletonImage, skeletonPoint[i][j], 3, cvScalar(0, 255, 255), 1, 8, 0); 
            }   
            drawSkeleton(skeletonImage, skeletonPoint[i], i); 
        } 
    }   
} 
//传入关节点位置画出人物骨架
void drawSkeleton(Mat &image, CvPoint pointSet[], int whichone) 
{ 
    CvScalar color; 
    switch(whichone) //跟踪不同的人显示不同的颜色 
    { 
		case 0:		color = cvScalar(255, 0, 0);		break; 
		case 1:		color = cvScalar(0,255, 0);		break; 
		case 2:		color = cvScalar(0, 0, 255);		break; 
		case 3:		color = cvScalar(255, 255, 0);	break; 
		case 4:		color = cvScalar(255, 0, 255);	break; 
		case 5:		color = cvScalar(0, 255, 255);	break; 
    } 
 
	// 躯干
    if((pointSet[NUI_SKELETON_POSITION_HEAD].x!=0 || pointSet[NUI_SKELETON_POSITION_HEAD].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].x!=0 || pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_HEAD], pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].x!=0 || pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_SPINE].x!=0 || pointSet[NUI_SKELETON_POSITION_SPINE].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER], pointSet[NUI_SKELETON_POSITION_SPINE], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_SPINE].x!=0 || pointSet[NUI_SKELETON_POSITION_SPINE].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_HIP_CENTER].x!=0 || pointSet[NUI_SKELETON_POSITION_HIP_CENTER].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_SPINE], pointSet[NUI_SKELETON_POSITION_HIP_CENTER], color, 2); 

    // 左上肢 
    if((pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].x!=0 || pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_SHOULDER_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_SHOULDER_LEFT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER], pointSet[NUI_SKELETON_POSITION_SHOULDER_LEFT], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_SHOULDER_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_SHOULDER_LEFT].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_ELBOW_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_ELBOW_LEFT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_SHOULDER_LEFT], pointSet[NUI_SKELETON_POSITION_ELBOW_LEFT], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_ELBOW_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_ELBOW_LEFT].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_WRIST_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_WRIST_LEFT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_ELBOW_LEFT], pointSet[NUI_SKELETON_POSITION_WRIST_LEFT], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_WRIST_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_WRIST_LEFT].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_HAND_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_HAND_LEFT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_WRIST_LEFT], pointSet[NUI_SKELETON_POSITION_HAND_LEFT], color, 2); 

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//right hand part
		double dRX=pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT].x-pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT].x;
		double dRY=pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT].y-pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT].y;
		gRH2Elength=sqrtl(dRX*dRX+dRY*dRY);
		gCurRhandX=pointSet[NUI_SKELETON_POSITION_HAND_RIGHT].x;
		gCurRhandY=pointSet[NUI_SKELETON_POSITION_HAND_RIGHT].y;
		//left hand part
		double dLX=pointSet[NUI_SKELETON_POSITION_WRIST_LEFT].x-pointSet[NUI_SKELETON_POSITION_ELBOW_LEFT].x;
		double dLY=pointSet[NUI_SKELETON_POSITION_WRIST_LEFT].y-pointSet[NUI_SKELETON_POSITION_ELBOW_LEFT].y;
		gLH2Elength=sqrtl(dLX*dLX+dLY*dLY);
		gCurLhandX=pointSet[NUI_SKELETON_POSITION_HAND_LEFT].x;
		gCurLhandY=pointSet[NUI_SKELETON_POSITION_HAND_LEFT].y;
		//head and shoulder center
		int headY=pointSet[NUI_SKELETON_POSITION_HEAD].y;
		int shoulderCenterY=pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].y;
		gCurEyeY=headY/2+shoulderCenterY/2;
		gCurColor=color;
		//head to left hand length
		double dH2LhX=pointSet[NUI_SKELETON_POSITION_HAND_LEFT].x-pointSet[NUI_SKELETON_POSITION_HEAD].x;
		double dH2LhY=pointSet[NUI_SKELETON_POSITION_HAND_LEFT].y-pointSet[NUI_SKELETON_POSITION_HEAD].y;
		gH2Lhlength=sqrtl(dH2LhX*dH2LhX+dH2LhY*dH2LhY);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		gHipCenterY=pointSet[NUI_SKELETON_POSITION_HIP_CENTER].y;

		//right
		gRE2HTheta=getAtan(pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT], pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT]);
		if(gRE2HTheta > -60 && gRE2HTheta < -45 && gHipCenterY< gCurLhandY)
		{
			gRSeqNum=0;
		}
		if(gRSeqNum < SEQ_NUM)
		{
			gRHandX[gRSeqNum]=pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT].x;
			gRElbowX[gRSeqNum]=pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT].x;
			gRSeqNum++;
		}

		//left
		gLE2HTheta=getAtan(pointSet[NUI_SKELETON_POSITION_ELBOW_LEFT], pointSet[NUI_SKELETON_POSITION_WRIST_LEFT]);
		if(gLE2HTheta > -135 && gLE2HTheta < -120 && gHipCenterY< gCurRhandY)
		{
			gLSeqNum=0;
		}
		if(gLSeqNum < SEQ_NUM)
		{
			gLHandX[gLSeqNum]=pointSet[NUI_SKELETON_POSITION_WRIST_LEFT].x;
			gLElbowX[gLSeqNum]=pointSet[NUI_SKELETON_POSITION_ELBOW_LEFT].x;
			gLSeqNum++;
		}

		//updown
		if(gRE2HTheta > -10 && gRE2HTheta < 10 && gHipCenterY< gCurLhandY && gH2Lhlength > 3.5*gLH2Elength)
		{
			gRUpDownSeqNum=0;
		}
		if(gRUpDownSeqNum < SEQ_NUM)
		{
			gRHandY[gRUpDownSeqNum]=pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT].y;
			gRElbowY[gRUpDownSeqNum]=pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT].y;
			gRUpDownSeqNum++;
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // 右上肢 
    if((pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].x!=0 || pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_SHOULDER_CENTER], pointSet[NUI_SKELETON_POSITION_SHOULDER_RIGHT], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_SHOULDER_RIGHT], pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT].y!=0)) 
		 line(image, pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT], pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_HAND_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_HAND_RIGHT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT], pointSet[NUI_SKELETON_POSITION_HAND_RIGHT], color, 2); 
 
    // 左下肢 
    if((pointSet[NUI_SKELETON_POSITION_HIP_CENTER].x!=0 || pointSet[NUI_SKELETON_POSITION_HIP_CENTER].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_HIP_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_HIP_LEFT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_HIP_CENTER], pointSet[NUI_SKELETON_POSITION_HIP_LEFT], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_HIP_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_HIP_LEFT].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_KNEE_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_KNEE_LEFT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_HIP_LEFT], pointSet[NUI_SKELETON_POSITION_KNEE_LEFT], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_KNEE_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_KNEE_LEFT].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_ANKLE_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_ANKLE_LEFT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_KNEE_LEFT], pointSet[NUI_SKELETON_POSITION_ANKLE_LEFT], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_ANKLE_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_ANKLE_LEFT].y!=0) &&  
        (pointSet[NUI_SKELETON_POSITION_FOOT_LEFT].x!=0 || pointSet[NUI_SKELETON_POSITION_FOOT_LEFT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_ANKLE_LEFT], pointSet[NUI_SKELETON_POSITION_FOOT_LEFT], color, 2); 

    // 右下肢 
    if((pointSet[NUI_SKELETON_POSITION_HIP_CENTER].x!=0 || pointSet[NUI_SKELETON_POSITION_HIP_CENTER].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_HIP_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_HIP_RIGHT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_HIP_CENTER], pointSet[NUI_SKELETON_POSITION_HIP_RIGHT], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_HIP_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_HIP_RIGHT].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_KNEE_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_KNEE_RIGHT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_HIP_RIGHT], pointSet[NUI_SKELETON_POSITION_KNEE_RIGHT],color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_KNEE_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_KNEE_RIGHT].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_ANKLE_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_ANKLE_RIGHT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_KNEE_RIGHT], pointSet[NUI_SKELETON_POSITION_ANKLE_RIGHT], color, 2); 
    if((pointSet[NUI_SKELETON_POSITION_ANKLE_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_ANKLE_RIGHT].y!=0) && 
        (pointSet[NUI_SKELETON_POSITION_FOOT_RIGHT].x!=0 || pointSet[NUI_SKELETON_POSITION_FOOT_RIGHT].y!=0)) 
        line(image, pointSet[NUI_SKELETON_POSITION_ANKLE_RIGHT], pointSet[NUI_SKELETON_POSITION_FOOT_RIGHT], color, 2); 
} 
//draw the dynamic hand pos trajectory
void drawDynamicTrajectory(Mat &image)
{
	CDynaHandPos pdhp1, pdhp2;
	int pgs=plist.GetSize()-2;
	for (int i=0; i<pgs; i++)//draw lines between poses
	{
		pdhp1=*(CDynaHandPos*)plist.GetAt(i);
		pdhp2=*(CDynaHandPos*)plist.GetAt(i+1);
		//line(image, cvPoint(pdhp1.x, pdhp1.y), cvPoint(pdhp2.x, pdhp2.y), gCurColor, 5); 
		line(image, cvPoint(pdhp1.x, pdhp1.y), cvPoint(pdhp2.x, pdhp2.y), cvScalar(255, 255, 255), 5); 
	}
}
//node to save sampled points
typedef struct PT 
{
	int x;
	int y;
}PT;
double getAtan(CvPoint pt1, CvPoint pt2)
{
	double dy=pt2.y-pt1.y;
	double dx=pt2.x-pt1.x;
	double basicTheta;
	if(dx==0.0)
	{
		if (dy>0.0)
			return 90;
		else if(dy<0.0)
			return -90;
		else
			return 1111;
	}
	else if(dy==0.0)
	{
		if(dx>0.0)
			return 0;
		else if(dx<0.0)
			return 180;
	}
	else
		basicTheta=180/3.1415926535897*atan(dy/dx);

	if (dx<0 && dy>0)
		return basicTheta+180;
	else if(dx<0 && dy<0)
		return basicTheta-180;
	else
		return basicTheta;
}
//draw the sampled points
double drawSampledCircle(Mat &sampledImage, Mat &trajectoryImage, double fMin2, double fMax2, struct svm_model* svmodel, int firstClass)
{
	int m=SAMPLE_NUM;
	int pgs=plist.GetSize()-1-3;
	int r=pgs/m;
	int A=r*m;
	int B=r*(m-1)+(m+1)/2;
	int C=r*(m-1)+m;
	int D=(r+1)*m;
	PT *sampledPT;
	sampledPT=new PT[SAMPLE_NUM];
	CDynaHandPos pdhp;
	////test
	//ofstream outfile;
	//outfile.open("outSampled.txt");
	//outfile<<getAtan(cvPoint(0, 0), cvPoint(0, 0))<<endl;//1111原地不动
	//outfile<<getAtan(cvPoint(0, 0), cvPoint(1, 0))<<endl;//0
	//outfile<<getAtan(cvPoint(0, 0), cvPoint(1, 1))<<endl;//45
	//outfile<<getAtan(cvPoint(0, 0), cvPoint(0, 1))<<endl;//90
	//outfile<<getAtan(cvPoint(0, 0), cvPoint(-1, 1))<<endl;//135
	//outfile<<getAtan(cvPoint(0, 0), cvPoint(-1, 0))<<endl;//180
	//outfile<<getAtan(cvPoint(0, 0), cvPoint(-1, -1))<<endl;//-135
	//outfile<<getAtan(cvPoint(0, 0), cvPoint(0, -1))<<endl;//-90
	//outfile<<getAtan(cvPoint(0, 0), cvPoint(1, -1))<<endl;//-45
	//outfile.close();
	if (pgs>=A && pgs<B)
	{
		for (int i=0; i<SAMPLE_NUM; i++)
		{
			pdhp=*(CDynaHandPos*)plist.GetAt(r*i+3);
			sampledPT[i].x=pdhp.x;
			sampledPT[i].y=pdhp.y;
		}
	} 
	else if (pgs>=B && pgs<C)
	{
		for (int i=0; i<SAMPLE_NUM; i++)
		{
			switch (i%2)
			{
			case 0:
				pdhp=*(CDynaHandPos*)plist.GetAt((2*r+1)*i/2+3);
				sampledPT[i].x=pdhp.x;
				sampledPT[i].y=pdhp.y;
				break;
			case 1:
				pdhp=*(CDynaHandPos*)plist.GetAt((2*r+1)*(i-1)/2+r+3);
				sampledPT[i].x=pdhp.x;
				sampledPT[i].y=pdhp.y;
				break;
			}
		}
	}
	else if (pgs>=C && pgs<D)
	{
		for (int i=0; i<SAMPLE_NUM; i++)
		{
			pdhp=*(CDynaHandPos*)plist.GetAt((r+1)*i+3);
			sampledPT[i].x=pdhp.x;
			sampledPT[i].y=pdhp.y;
		}
	}

	//ofstream outfile;
	//outfile.open("outSampled.txt");

	CvScalar inverseColor;
	inverseColor.val[0]=255-gCurColor.val[0];
	inverseColor.val[1]=255-gCurColor.val[1];
	inverseColor.val[2]=255-gCurColor.val[2];
	if(firstClass==F_CLOCK)
	{
		for (int i=0; i<SAMPLE_NUM-1; i++)
		{
			line(sampledImage, cvPoint(sampledPT[i].x, sampledPT[i].y), cvPoint(sampledPT[i+1].x, sampledPT[i+1].y), gCurColor, 5); 
			//outfile<<i+1<<":"<<(int)getAtan(cvPoint(sampledPT[i].x, sampledPT[i].y), cvPoint(sampledPT[i+1].x, sampledPT[i+1].y))<<'\t';

			svmX_DynamicHand[i].index=i+1;
			int tempTheta=(int)getAtan(cvPoint(sampledPT[i].x, sampledPT[i].y), cvPoint(sampledPT[i+1].x, sampledPT[i+1].y));
			svmX_DynamicHand[i].value=OnScaleValue(fMin2, fMax2, mySn_dynamic_clock[i].minv, mySn_dynamic_clock[i].maxv, tempTheta);//dynamic svm data
			circle(sampledImage, cvPoint(sampledPT[i].x, sampledPT[i].y), 5, inverseColor, 2, 8, 0); 
			circle(trajectoryImage, cvPoint(sampledPT[i].x, sampledPT[i].y), 5, gCurColor, 2, 8, 0); 
			if (i==SAMPLE_NUM-2)
			{
				circle(sampledImage, cvPoint(sampledPT[i+1].x, sampledPT[i+1].y), 5, inverseColor, 2, 8, 0); 
				circle(trajectoryImage, cvPoint(sampledPT[i+1].x, sampledPT[i+1].y), 5, gCurColor, 2, 8, 0); 
			}
		}
	}
	else if(firstClass==F_NEARFAR)
	{
		for (int i=0; i<SAMPLE_NUM-1; i++)
		{
			line(sampledImage, cvPoint(sampledPT[i].x, sampledPT[i].y), cvPoint(sampledPT[i+1].x, sampledPT[i+1].y), gCurColor, 5); 
			//outfile<<i+1<<":"<<(int)getAtan(cvPoint(sampledPT[i].x, sampledPT[i].y), cvPoint(sampledPT[i+1].x, sampledPT[i+1].y))<<'\t';

			svmX_DynamicHand[i].index=i+1;
			int tempTheta=(int)getAtan(cvPoint(sampledPT[i].x, sampledPT[i].y), cvPoint(sampledPT[i+1].x, sampledPT[i+1].y));
			svmX_DynamicHand[i].value=OnScaleValue(fMin2, fMax2, mySn_dynamic_nearfar[i].minv, mySn_dynamic_nearfar[i].maxv, tempTheta);//dynamic svm data
			circle(sampledImage, cvPoint(sampledPT[i].x, sampledPT[i].y), 5, inverseColor, 2, 8, 0); 
			circle(trajectoryImage, cvPoint(sampledPT[i].x, sampledPT[i].y), 5, gCurColor, 2, 8, 0); 
			if (i==SAMPLE_NUM-2)
			{
				circle(sampledImage, cvPoint(sampledPT[i+1].x, sampledPT[i+1].y), 5, inverseColor, 2, 8, 0); 
				circle(trajectoryImage, cvPoint(sampledPT[i+1].x, sampledPT[i+1].y), 5, gCurColor, 2, 8, 0); 
			}
		}
	}
	svmX_DynamicHand[SAMPLE_NUM-1].index=-1;

	delete sampledPT;
	//outfile.close();
	double predict_label = svm_predict(svmodel, svmX_DynamicHand);
	return predict_label;
}
//clearn a specified Mat
void cvMySetMatEmpty(Mat &image)
{
	for (int i=0; i<image.rows; i++)
	{
		uchar *depthhead = image.ptr<uchar>(i);//point of the row i , one uchar indicates one color
		for (int j=0; j<image.cols; j++)
		{
			depthhead[3*j] = 0;
			depthhead[3*j+1] = 0;
			depthhead[3*j+2] = 0;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


DWORD WINAPI OnProcessDepth(LPVOID lpParam)
{
	IplImage *RBinary=0;
	IplImage *LBinary=0;
	IplImage ipl_depth;
	IplImage ipl_skeleton;
	IplImage ipl_trajectory;
	IplImage ipl_sampled;
	CvScalar stemp;
	CvRect rect;
	bool bIsStarted=false;
	double handRecogResult[STATIC_NUM]={100};
	int counter=0;

	//static hand
	std::ifstream infile;
	infile.open("res\\staticNormalization.range");//读入训练得到的全局规格化文件
	char chX;
	double fMin, fMax;
	infile>>chX>>fMin>>fMax;
	for (int k=0; k<7; k++)
	{
		infile>>mySn_static_hand[k].element>>mySn_static_hand[k].minv>>mySn_static_hand[k].maxv;
	}
	infile.close();
	static_model=svm_load_model("res\\staticHand.model");//加载训练得到的svm模型
	svmX_StaticHand = (struct svm_node *) malloc(max_nr_attr_static_hand*sizeof(struct svm_node));//svmX_StaticHand为libSVM定义的节点（index：value）

	//dynamic clock hand
	infile.open("res\\clock.range");
	char chX2;
	double fMin2, fMax2;
	infile>>chX2>>fMin2>>fMax2;
	for (int k=0; k<16; k++)
	{
		infile>>mySn_dynamic_clock[k].element>>mySn_dynamic_clock[k].minv>>mySn_dynamic_clock[k].maxv;
	}
	infile.close();
	clock_model=svm_load_model("res\\clock.model");

	//dynamic nearfar hand
	infile.open("res\\nearfar.range");
	char chX3;
	double fMin3, fMax3;
	infile>>chX3>>fMin3>>fMax3;
	for (int k=0; k<16; k++)
	{
		infile>>mySn_dynamic_nearfar[k].element>>mySn_dynamic_nearfar[k].minv>>mySn_dynamic_nearfar[k].maxv;
	}
	infile.close();
	nearfar_model=svm_load_model("res\\nearfar.model");

	svmX_DynamicHand = (struct svm_node *) malloc(max_nr_attr_dynamic_hand*sizeof(struct svm_node));

	//useful for template comparing 
	int minTrajectoryX=500, minTrajectoryY=500, maxTrajectoryX=0, maxTrajectoryY=0;
	IplImage * clock_average=0;
	IplImage * comenear_average=0;
	IplImage * gofar_average=0;
	IplImage * s_average=0;
	clock_average=cvLoadImage("res\\clock_average.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	comenear_average=cvLoadImage("res\\comenear_average.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	gofar_average=cvLoadImage("res\\gofar_average.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	s_average=cvLoadImage("res\\s_average.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	IplImage *dstRGB=0;
	IplImage * dstGray=0;

	//int nFileIndex=0;
	//ofstream outfile;
	//outfile.open("dyGesture.txt");

	while(true)
	{
		// 无限等待新的数据，等到后WaitForSingleObject为0，则返回
		if(WaitForSingleObject(pMyDlg->m_hNextDepthFrameEvent, 0)==0) 
			getDepthImage(pMyDlg->m_hNextDepthFrameEvent, pMyDlg->m_pDepthStreamHandle, pMyDlg->m_depthImage);  
		if(WaitForSingleObject(pMyDlg->m_skeletonEvent, INFINITE)==0)
			getSkeletonImage(pMyDlg->m_skeletonEvent, pMyDlg->m_skeletonImage); 

		ipl_depth = pMyDlg->m_depthImage;
		pMyDlg->OnShowImage(&ipl_depth, IDC_FRAME_DEPTH);
		ipl_skeleton = pMyDlg->m_skeletonImage;
		pMyDlg->OnShowImage(&ipl_skeleton, IDC_FRAME_SKELETON);

		//char strSaveFileName[20];
		//sprintf(strSaveFileName, "depth_%d.jpg", nFileIndex);
		//cvSaveImage(strSaveFileName, &ipl_depth);
		//sprintf(strSaveFileName, "skeleton_%d.jpg", nFileIndex);
		//cvSaveImage(strSaveFileName, &ipl_skeleton);
		//nFileIndex++;

		//right hand part
		if((gRH2Elength!=0) && (gCurRhandX-gRH2Elength*0.6>0) && (gCurRhandY-gRH2Elength*0.6>0) && ((int)(gCurRhandX+gRH2Elength*0.6)<ipl_depth.width) && ((int)(gCurRhandY+gRH2Elength*0.6)<ipl_depth.height))
		{
			rect.x=(int)(gCurRhandX-gRH2Elength*0.6);
			rect.y=(int)(gCurRhandY-gRH2Elength*0.6);
			rect.width=(int)(gRH2Elength*1.2);
			rect.height=(int)(gRH2Elength*1.2);
			cvSetImageROI(&ipl_depth, rect);

			RBinary=cvCreateImage(cvGetSize(&ipl_depth), IPL_DEPTH_8U, 1);
			for (int i=0; i<RBinary->height; i++)
			{
				for (int j=0; j<RBinary->width; j++)
				{
					((uchar *)(RBinary->imageData+i*RBinary->widthStep))[j]=0;
				}
			}
			for (int i=0; i<RBinary->height; i++)
			{
				for (int j=0; j<RBinary->width; j++)
				{
					stemp=cvGet2D(&ipl_depth, i, j);
					if (abs(stemp.val[0]-gCurColor.val[0])<=20 && abs(stemp.val[1]-gCurColor.val[1])<=20 && abs(stemp.val[2]-gCurColor.val[2])<=20)
					{
						((uchar *)(RBinary->imageData+i*RBinary->widthStep))[j]=255;
					}
				}
			}
			pMyDlg->OnShowImage(RBinary, IDC_FRAME_RHAND);
			//double predict_label=GetStaticHandSVMRecogResult(RBinary, fMin, fMax);
			//if (nFileIndex%2==0)//write binary hand pic file
			//{
			//	char strSaveFileName[20];
			//	sprintf(strSaveFileName, "2_%d.jpg", nFileIndex/2);
			//	cvSaveImage(strSaveFileName, RBinary );
			//}
			//nFileIndex++;
			cvResetImageROI(&ipl_depth);
			cvReleaseImage(&RBinary);
		}

		//left hand part
		if((gLH2Elength!=0) && (gCurLhandX-gLH2Elength*0.6>0) && (gCurLhandY-gLH2Elength*0.6>0) && ((int)(gCurLhandX+gLH2Elength*0.6)<ipl_depth.width) && ((int)(gCurLhandY+gLH2Elength*0.6)<ipl_depth.height))
		{
			rect.x=(int)(gCurLhandX-gLH2Elength*0.6);
			rect.y=(int)(gCurLhandY-gLH2Elength*0.6);
			rect.width=(int)(gLH2Elength*1.2);
			rect.height=(int)(gLH2Elength*1.2);
			cvSetImageROI(&ipl_depth, rect);

			LBinary=cvCreateImage(cvGetSize(&ipl_depth), IPL_DEPTH_8U, 1);
			for (int i=0; i<LBinary->height; i++)
			{
				for (int j=0; j<LBinary->width; j++)
				{
					((uchar *)(LBinary->imageData+i*LBinary->widthStep))[j]=0;
				}
			}
			for (int i=0; i<LBinary->height; i++)
			{
				for (int j=0; j<LBinary->width; j++)
				{
					stemp=cvGet2D(&ipl_depth, i, j);
					if (abs(stemp.val[0]-gCurColor.val[0])<=20 && abs(stemp.val[1]-gCurColor.val[1])<=20 && abs(stemp.val[2]-gCurColor.val[2])<=20)
					{
						((uchar *)(LBinary->imageData+i*LBinary->widthStep))[j]=255;
					}
				}
			}
			pMyDlg->OnShowImage(LBinary, IDC_FRAME_LHAND);
			if (gCurLhandY<gCurEyeY)
			{
				double predict_label=GetStaticHandSVMRecogResult(LBinary, fMin, fMax);
				handRecogResult[counter]=predict_label-1;
				counter++;
				if(counter==STATIC_NUM)
				{
					double total=0;
					for (int i=0; i<counter; i++)
					{
						total+=handRecogResult[i];
					}
					if(total==0)//if start
					{
						bDynamicStart=true;
						bDynamicStop=false;
						ipl_SOS_label = cvLoadImage("res\\start.jpg");//show start label
						pMyDlg->OnShowImage(ipl_SOS_label, IDC_FRAME_SOS_LABEL);
						cvReleaseImage(&ipl_SOS_label);
						Sleep(20);
					}
					else if (total==STATIC_NUM)//if stop
					{
						bDynamicStart=false;
						bDynamicStop=true;
						ipl_SOS_label = cvLoadImage("res\\stop.jpg");//show stop label
						pMyDlg->OnShowImage(ipl_SOS_label, IDC_FRAME_SOS_LABEL);
						cvReleaseImage(&ipl_SOS_label);
					}
					counter=0;
				}
			}
			else
			{
				counter=0;
			}

			//if (nFileIndex%2==0)//write binary hand pic file
			//{
			//	char strSaveFileName[20];
			//	sprintf(strSaveFileName, "2_%d.jpg", nFileIndex/2);
			//	cvSaveImage(strSaveFileName, LBinary );
			//}
			//nFileIndex++;
			cvResetImageROI(&ipl_depth);
			cvReleaseImage(&LBinary);
		}

		//when fist, bDynamicStart=false, bDynamicStop=true
		//when palm, bDynamicStart=true, bDynamicStop=false
		if (bDynamicStart && !bDynamicStop)//if start
		{
			pDhp=new CDynaHandPos(gCurRhandX, gCurRhandY);//add right hand pos to a list
			//pDhp=new CDynaHandPos(gCurLhandX, gCurLhandY);//add left hand pos to a list
			plist.Add(pDhp);
			if(gCurRhandX<minTrajectoryX) minTrajectoryX=gCurRhandX;//useful for get the accurate trajectory region
			if(gCurRhandY<minTrajectoryY) minTrajectoryY=gCurRhandY;
			if(gCurRhandX>maxTrajectoryX) maxTrajectoryX=gCurRhandX;
			if(gCurRhandY>maxTrajectoryY) maxTrajectoryY=gCurRhandY;

			//adding the following three can draw dynamic trajectory
			drawDynamicTrajectory(pMyDlg->m_trajectoryImage);
			ipl_trajectory = pMyDlg->m_trajectoryImage;
			pMyDlg->OnShowImage(&ipl_trajectory, IDC_FRAME_TRAJECTORY);
			bIsStarted=true;
		}
		else if (!bDynamicStart && bDynamicStop)//if stop
		{
			bDynamicStart=false;
			bDynamicStop=false;
			if (bIsStarted)
			{
				drawDynamicTrajectory(pMyDlg->m_trajectoryImage);//draw trajectory according to the hand pos list

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////get the accurate region of the trajectory
				ipl_trajectory = pMyDlg->m_trajectoryImage;
				CvRect rectTrajectory;
				rectTrajectory.x=minTrajectoryX;
				rectTrajectory.y=minTrajectoryY;
				rectTrajectory.width=maxTrajectoryX-minTrajectoryX;
				rectTrajectory.height=maxTrajectoryY-minTrajectoryY;
				cvSetImageROI(&ipl_trajectory, rectTrajectory);
				dstRGB = cvCreateImage(cvSize(30, 30), (&ipl_trajectory)->depth, (&ipl_trajectory)->nChannels);	//构造缩小后的目标图象
				cvResize(&ipl_trajectory, dstRGB, CV_INTER_LINEAR);
				dstGray=cvCreateImage(cvGetSize(dstRGB), IPL_DEPTH_8U, 1);
				cvCvtColor(dstRGB, dstGray, CV_BGR2GRAY); 
				//cvSaveImage("dstgray.jpg", dstGray);

				double sumDelt=0;
				double firstResult[4]={-1};

				//compare with template clock
				for(int j=0; j<dstGray->width; j++)
				{
					for(int i=0; i<dstGray->height; i++)
					{
						sumDelt+=abs(cvGet2D(clock_average, i, j).val[0]-cvGet2D(dstGray, i, j).val[0]);
					}
				}
				firstResult[0]=sumDelt/900.0;

				//compare with template comenear
				sumDelt=0;
				for(int j=0; j<dstGray->width; j++)
				{
					for(int i=0; i<dstGray->height; i++)
					{
						sumDelt+=abs(cvGet2D(comenear_average, i, j).val[0]-cvGet2D(dstGray, i, j).val[0]);
					}
				}
				firstResult[1]=sumDelt/900.0;

				//compare with template gofar
				sumDelt=0;
				for(int j=0; j<dstGray->width; j++)
				{
					for(int i=0; i<dstGray->height; i++)
					{
						sumDelt+=abs(cvGet2D(gofar_average, i, j).val[0]-cvGet2D(dstGray, i, j).val[0]);
					}
				}
				firstResult[2]=sumDelt/900.0;

				//compare with template s
				sumDelt=0;
				for(int j=0; j<dstGray->width; j++)
				{
					for(int i=0; i<dstGray->height; i++)
					{
						sumDelt+=abs(cvGet2D(s_average, i, j).val[0]-cvGet2D(dstGray, i, j).val[0]);
					}
				}
				firstResult[3]=sumDelt/900.0;

				double dbmin=100;
				int index=-1;
				for (int i=0; i<4; i++)
				{
					if (dbmin>firstResult[i])
					{
						dbmin=firstResult[i];
						index=i;
					}
				}
				//if(dbmin>57)//binary
				//	index=4;
				if(dbmin>55)//gray
					index=4;

				double predict_dynamic_label=-1;
				switch(index)
				{
				case 0:
					predict_dynamic_label=drawSampledCircle(pMyDlg->m_sampledImage, pMyDlg->m_trajectoryImage, fMin2, fMax2, clock_model, F_CLOCK);
					break;
				case 1:
					drawSampledCircle(pMyDlg->m_sampledImage, pMyDlg->m_trajectoryImage, fMin3, fMax3, nearfar_model, F_NEARFAR);
					predict_dynamic_label=4444;	
					//cout<<"COME NEAR"<<endl;
					break;
				case 2:
					drawSampledCircle(pMyDlg->m_sampledImage, pMyDlg->m_trajectoryImage, fMin3, fMax3, nearfar_model, F_NEARFAR);
					predict_dynamic_label=3333;	
					//cout<<"GO FAR"<<endl;
					break;
				case 3:
					predict_dynamic_label=5555;	
					//cout<<"S"<<endl;
					break;
				case 4:
					predict_dynamic_label=6666;
					//cout<<"NONE"<<endl;
					break;
				}

				cvReleaseImage(&dstRGB);
				cvReleaseImage(&dstGray);
				cvResetImageROI(&ipl_trajectory);
				minTrajectoryX=500;
				minTrajectoryY=500;
				maxTrajectoryX=0;
				maxTrajectoryY=0;
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//double predict_dynamic_label=drawSampledCircle(pMyDlg->m_sampledImage, pMyDlg->m_trajectoryImage, fMin2, fMax2);
		
				ipl_trajectory = pMyDlg->m_trajectoryImage;
				pMyDlg->OnShowImage(&ipl_trajectory, IDC_FRAME_TRAJECTORY);//just show the trajectory
				ipl_sampled = pMyDlg->m_sampledImage;
				pMyDlg->OnShowImage(&ipl_sampled, IDC_FRAME_SAMPLED_POINT);

				switch ((int)predict_dynamic_label)
				{
				case 1111: 
						gRECOG_RESULT=COUNTERCLOCK;
						ipl_dynamicResult = cvLoadImage("res\\counterclock.jpg");
						pMyDlg->OnShowImage(ipl_dynamicResult, IDC_FRAME_DYNAMIC_RESULT);
						cvReleaseImage(&ipl_dynamicResult);
						break;
				case 2222:
						gRECOG_RESULT=CLOCK;
						ipl_dynamicResult = cvLoadImage("res\\clock.jpg");
						pMyDlg->OnShowImage(ipl_dynamicResult, IDC_FRAME_DYNAMIC_RESULT);
						cvReleaseImage(&ipl_dynamicResult);
						/////////////////////////////////////////////////////////////////
						ipl_phoneCall = cvLoadImage("res\\endcall.jpg");
						pMyDlg->OnShowImage(ipl_phoneCall, IDC_FRAME_PHONE_CALL);
						cvReleaseImage(&ipl_phoneCall);
						/////////////////////////////////////////////////////////////////
						break;
				case 3333:
						gRECOG_RESULT=GOFAR;
						ipl_dynamicResult = cvLoadImage("res\\gothere.jpg");
						pMyDlg->OnShowImage(ipl_dynamicResult, IDC_FRAME_DYNAMIC_RESULT);
						cvReleaseImage(&ipl_dynamicResult);
						break;
				case 4444:
						gRECOG_RESULT=COMENEAR;
						ipl_dynamicResult = cvLoadImage("res\\comehere.jpg");
						pMyDlg->OnShowImage(ipl_dynamicResult, IDC_FRAME_DYNAMIC_RESULT);
						cvReleaseImage(&ipl_dynamicResult);
						break;
				case 5555:
						gRECOG_RESULT=TOSTOP;
						ipl_dynamicResult = cvLoadImage("res\\dstop.jpg");
						pMyDlg->OnShowImage(ipl_dynamicResult, IDC_FRAME_DYNAMIC_RESULT);
						cvReleaseImage(&ipl_dynamicResult);
						break;
				case 6666:
						gRECOG_RESULT=NONE;
						ipl_dynamicResult = cvLoadImage("res\\question.jpg");
						pMyDlg->OnShowImage(ipl_dynamicResult, IDC_FRAME_DYNAMIC_RESULT);
						cvReleaseImage(&ipl_dynamicResult);
						break;
				}
				//if (plist.GetSize()!=0)
				//{
				//	CDynaHandPos pdhp;
				//	outfile<<plist.GetSize()-1<<endl;
				//	for (int k=0; k<plist.GetSize()-1; k++)//write the poses into file for futher use
				//	{
				//		pdhp=*(CDynaHandPos*)plist.GetAt(k);
				//		outfile<<pdhp.x<<'\t'<<pdhp.y<<'\t'<<'\t';
				//	}
				//	outfile<<endl<<endl;
				//}

				plist.RemoveAll();
				cvMySetMatEmpty(pMyDlg->m_trajectoryImage);//clean the trajectory canvs
				cvMySetMatEmpty(pMyDlg->m_sampledImage);
				bIsStarted=false;
			}
		}

		if(gCurLhandY > gHipCenterY || gCurLhandY < gCurEyeY)
			gRUpDownSeqNum=0;

	}
	//outfile.close();
	cvReleaseImage(&clock_average);
	cvReleaseImage(&comenear_average);
	cvReleaseImage(&gofar_average);
	cvReleaseImage(&s_average);
	return 0;
}


DWORD WINAPI OnListeningHand(LPVOID lpParam)
{
	IplImage *ipl_show=0;
	ipl_show = cvLoadImage(path[0]);
	pMyDlg->OnShowImage(ipl_show, IDC_FRAME_SHOW);
	cvReleaseImage(&ipl_show);
	int nPicOrder=0;

	ipl_SOS_label = cvLoadImage("res\\question.jpg");
	pMyDlg->OnShowImage(ipl_SOS_label, IDC_FRAME_SOS_LABEL);
	cvReleaseImage(&ipl_SOS_label);

	ipl_dynamicResult = cvLoadImage("res\\question.jpg");
	pMyDlg->OnShowImage(ipl_dynamicResult, IDC_FRAME_DYNAMIC_RESULT);
	cvReleaseImage(&ipl_dynamicResult);

	ipl_phoneCall = cvLoadImage("res\\endcall.jpg");
	pMyDlg->OnShowImage(ipl_phoneCall, IDC_FRAME_PHONE_CALL);
	cvReleaseImage(&ipl_phoneCall);

	while(true)
	{
		//right to left slider show
		if((gRHandX[SEQ_NUM-1]!=-1) && (gRElbowX[SEQ_NUM-1]!=-1))
		{
			if(gHipCenterY< gCurLhandY && gRE2HTheta < -150 && gRSeqNum==SEQ_NUM)
			{
				for(int i=0; i<SEQ_NUM-1; i++)
				{
					if((gRHandX[i+1]-gRHandX[i] < 0) && (gRElbowX[i+1]-gRElbowX[i] < 0))
						gRGuessRate++;
				}
				if(gRGuessRate > RECOGNIZE_RATE*SEQ_NUM)
				{
					gRECOG_RESULT=TOLEFT;
					ipl_dynamicResult = cvLoadImage("res\\left.jpg");
					pMyDlg->OnShowImage(ipl_dynamicResult, IDC_FRAME_DYNAMIC_RESULT);
					cvReleaseImage(&ipl_dynamicResult);

					if (nPicOrder==7)
					{
						nPicOrder=-1;
					}
					nPicOrder++;
					ipl_show = cvLoadImage(path[nPicOrder]);
					pMyDlg->OnShowImage(ipl_show, IDC_FRAME_SHOW);
					cvReleaseImage(&ipl_show);
				}
				gRSeqNum=0;
				gRGuessRate=0;
				gRUpDownSeqNum=0;
			}
		}
		else
		{
			Sleep(10);
		}

		//left to right slider show
		if((gLHandX[SEQ_NUM-1]!=-1) && (gLElbowX[SEQ_NUM-1]!=-1))
		{
			if(gHipCenterY< gCurRhandY && gLE2HTheta < 0 && gLE2HTheta > -30 && gLSeqNum==SEQ_NUM)
			{
				for(int i=0; i<SEQ_NUM-1; i++)
				{
					if((gLHandX[i+1]-gLHandX[i] >0) && (gLElbowX[i+1]-gLElbowX[i] > 0))
						gLGuessRate++;
				}
				if(gLGuessRate > RECOGNIZE_RATE*SEQ_NUM)
				{
					gRECOG_RESULT=TORIGHT;
					ipl_dynamicResult = cvLoadImage("res\\right.jpg");
					pMyDlg->OnShowImage(ipl_dynamicResult, IDC_FRAME_DYNAMIC_RESULT);
					cvReleaseImage(&ipl_dynamicResult);

					if (nPicOrder==0)
					{
						nPicOrder=8;
					}
					nPicOrder--;
					ipl_show = cvLoadImage(path[nPicOrder]);
					pMyDlg->OnShowImage(ipl_show, IDC_FRAME_SHOW);
					cvReleaseImage(&ipl_show);
				}
				gLSeqNum=0;
				gLGuessRate=0;
				gRUpDownSeqNum=0;
			}
		}
		else
		{
			Sleep(10);
		}

		//updown
		if((gRHandY[SEQ_NUM-1]!=-1) && (gRElbowY[SEQ_NUM-1]!=-1))
		{
			//up
			if(gCurLhandY > gCurEyeY  && gCurLhandY < gHipCenterY && gRE2HTheta < -60 && gRSeqNum==SEQ_NUM && gH2Lhlength > 3.5*gLH2Elength)
			{
				for(int i=0; i<SEQ_NUM-1; i++)
				{
					if((gRHandY[i+1]-gRHandY[i] < 0) && (gRElbowY[i+1]-gRElbowY[i] < 0))
						gRUpGuessRate++;
				}
				if(gRUpGuessRate > RECOGNIZE_RATE*SEQ_NUM)
				{
					gRECOG_RESULT=UP;
					ipl_dynamicResult = cvLoadImage("res\\up.jpg");
					pMyDlg->OnShowImage(ipl_dynamicResult, IDC_FRAME_DYNAMIC_RESULT);
					cvReleaseImage(&ipl_dynamicResult);

					//if (nPicOrder==7)
					//{
					//	nPicOrder=-1;
					//}
					//nPicOrder++;
					//ipl_show = cvLoadImage(path[nPicOrder]);
					//pMyDlg->OnShowImage(ipl_show, IDC_FRAME_SHOW);
					//cvReleaseImage(&ipl_show);
				}
				gRUpDownSeqNum=0;
				gRUpGuessRate=0;
			}

			//down
			if(gCurLhandY > gCurEyeY  && gCurLhandY < gHipCenterY && gRE2HTheta > 45 && gRUpDownSeqNum==SEQ_NUM && gH2Lhlength > 3.5*gLH2Elength) 
			{
				for(int i=0; i<SEQ_NUM-1; i++)
				{
					if((gRHandY[i+1]-gRHandY[i] > 0) && (gRElbowY[i+1]-gRElbowY[i] > 0))
						gRDownGuessRate++;
				}
				if(gRDownGuessRate > RECOGNIZE_RATE*SEQ_NUM)
				{
					gRECOG_RESULT=DOWN;
					ipl_dynamicResult = cvLoadImage("res\\down.jpg");
					pMyDlg->OnShowImage(ipl_dynamicResult, IDC_FRAME_DYNAMIC_RESULT);
					cvReleaseImage(&ipl_dynamicResult);

					//if (nPicOrder==7)
					//{
					//	nPicOrder=-1;
					//}
					//nPicOrder++;
					//ipl_show = cvLoadImage(path[nPicOrder]);
					//pMyDlg->OnShowImage(ipl_show, IDC_FRAME_SHOW);
					//cvReleaseImage(&ipl_show);
				}
				gRUpDownSeqNum=0;
				gRDownGuessRate=0;
			}
		}
		else
		{
			Sleep(10);
		}

		Sleep(10);
	}
	return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////recv cmd from WiFi
char RecvBuffer[RECV_BUFFER_SIZE];
DWORD WINAPI OnListeningCommand(LPVOID lpParam)
{
	SOCKET sockRecv=(int)lpParam;
	recv(sockRecv, RecvBuffer, RECV_BUFFER_SIZE, 0);
	RecvBuffer[strlen(RecvBuffer)]='\0';

	//label the start and stop of dynamic gesture
	if((CString)RecvBuffer=="START")//if start
	{
		bDynamicStart=true;
		bDynamicStop=false;
		ipl_SOS_label = cvLoadImage("res\\start.jpg");//show start label
		pMyDlg->OnShowImage(ipl_SOS_label, IDC_FRAME_SOS_LABEL);
		cvReleaseImage(&ipl_SOS_label);
	}
	else if ((CString)RecvBuffer=="STOP")//if stop
	{
		bDynamicStart=false;
		bDynamicStop=true;
		ipl_SOS_label = cvLoadImage("res\\stop.jpg");//show stop label
		pMyDlg->OnShowImage(ipl_SOS_label, IDC_FRAME_SOS_LABEL);
		cvReleaseImage(&ipl_SOS_label);
	}

	else if ((CString)RecvBuffer=="up")
	{
		gRECOG_RESULT=UP;
		ipl_dynamicResult = cvLoadImage("res\\up.jpg");
		pMyDlg->OnShowImage(ipl_dynamicResult, IDC_FRAME_DYNAMIC_RESULT);
		cvReleaseImage(&ipl_dynamicResult);
	}
	else if ((CString)RecvBuffer=="down")
	{
		gRECOG_RESULT=DOWN;
		ipl_dynamicResult = cvLoadImage("res\\down.jpg");
		pMyDlg->OnShowImage(ipl_dynamicResult, IDC_FRAME_DYNAMIC_RESULT);
		cvReleaseImage(&ipl_dynamicResult);
	}
	else if ((CString)RecvBuffer=="left")
	{
		gRECOG_RESULT=TOLEFT;
		ipl_dynamicResult = cvLoadImage("res\\left.jpg");
		pMyDlg->OnShowImage(ipl_dynamicResult, IDC_FRAME_DYNAMIC_RESULT);
		cvReleaseImage(&ipl_dynamicResult);
	}
	else if ((CString)RecvBuffer=="right")
	{
		gRECOG_RESULT=TORIGHT;
		ipl_dynamicResult = cvLoadImage("res\\right.jpg");
		pMyDlg->OnShowImage(ipl_dynamicResult, IDC_FRAME_DYNAMIC_RESULT);
		cvReleaseImage(&ipl_dynamicResult);
	}
	else if ((CString)RecvBuffer=="cclock")
	{
		gRECOG_RESULT=COUNTERCLOCK;
		ipl_dynamicResult = cvLoadImage("res\\counterclock.jpg");
		pMyDlg->OnShowImage(ipl_dynamicResult, IDC_FRAME_DYNAMIC_RESULT);
		cvReleaseImage(&ipl_dynamicResult);
	}
	else if ((CString)RecvBuffer=="clock")
	{
		gRECOG_RESULT=CLOCK;
		ipl_dynamicResult = cvLoadImage("res\\clock.jpg");
		pMyDlg->OnShowImage(ipl_dynamicResult, IDC_FRAME_DYNAMIC_RESULT);
		cvReleaseImage(&ipl_dynamicResult);
	}
	else if ((CString)RecvBuffer=="far")
	{
		gRECOG_RESULT=GOFAR;
		ipl_dynamicResult = cvLoadImage("res\\gothere.jpg");
		pMyDlg->OnShowImage(ipl_dynamicResult, IDC_FRAME_DYNAMIC_RESULT);
		cvReleaseImage(&ipl_dynamicResult);
	}
	else if ((CString)RecvBuffer=="near")
	{
		gRECOG_RESULT=COMENEAR;
		ipl_dynamicResult = cvLoadImage("res\\comehere.jpg");
		pMyDlg->OnShowImage(ipl_dynamicResult, IDC_FRAME_DYNAMIC_RESULT);
		cvReleaseImage(&ipl_dynamicResult);
	}
	else if ((CString)RecvBuffer=="stop")
	{
		gRECOG_RESULT=TOSTOP;
		ipl_dynamicResult = cvLoadImage("res\\dstop.jpg");
		pMyDlg->OnShowImage(ipl_dynamicResult, IDC_FRAME_DYNAMIC_RESULT);
		cvReleaseImage(&ipl_dynamicResult);
	}
	else if ((CString)RecvBuffer=="phone_idle")
	{
		ipl_phoneCall = cvLoadImage("res\\endcall.jpg");
		pMyDlg->OnShowImage(ipl_phoneCall, IDC_FRAME_PHONE_CALL);
		cvReleaseImage(&ipl_phoneCall);
	}
	else if ((CString)RecvBuffer=="phone_ringing")
	{
		ipl_phoneCall = cvLoadImage("res\\ring.jpg");
		pMyDlg->OnShowImage(ipl_phoneCall, IDC_FRAME_PHONE_CALL);
		cvReleaseImage(&ipl_phoneCall);
	}

	memset(RecvBuffer, 0, sizeof(RecvBuffer));
	return 0;
}
DWORD WINAPI OnListingAccept(LPVOID lpParam)
{
	ipl_SOS_label = cvLoadImage("res\\question.jpg");//show the question label
	pMyDlg->OnShowImage(ipl_SOS_label, IDC_FRAME_SOS_LABEL);
	cvReleaseImage(&ipl_SOS_label);

	SOCKET temp, socket;
	socket=(int)lpParam;
	while(true)
	{
		temp=accept(socket, NULL, NULL);
		CreateThread(NULL, 0, OnListeningCommand, (void*)temp, 0, 0);
	}
	return 0;
}
bool CMyKinectTryDlg::OnOpenRecvSocket(int recvport)
{
	WSADATA wsadata;
	SOCKET sockRecv;
	if(WSAStartup(MAKEWORD(2, 2), &wsadata)!=0)
	{
		AfxMessageBox(_T("WSAStartup 失败!"));
		return FALSE;
	}
	sockRecv=socket(AF_INET, SOCK_STREAM, 0);
	while(sockRecv==INVALID_SOCKET)
	{
		if(AfxMessageBox(_T("SockRecv 初始化失败!\n重试?"), MB_YESNO)==IDNO)
			return false;
	}

	sockaddr_in sin;
	sin.sin_port=htons(recvport);
	sin.sin_family=AF_INET;
	sin.sin_addr.s_addr=0;
	while(bind(sockRecv, (sockaddr*)&sin, sizeof(sockaddr))==SOCKET_ERROR)
	{
		if(AfxMessageBox(_T("Bind 失败!\n重试?"), MB_YESNO)==IDNO)
			return false;
	}
	listen(sockRecv, 5);

	HANDLE handle=CreateThread(NULL, 0, OnListingAccept, (void*)sockRecv, 0, 0);
	if(!handle)
		MessageBox(_T("创建 Recv 线程失败!"), 0, 0);

	CloseHandle(handle);

	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
