#include <windows.h>
#include <iostream> 
#include <NuiApi.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int seqNum=0;
int guessRate=0;
int numSample=0;
#define SEQ_NUM 15
#define SAMPLE_DELT 1
#define RECOGNIZE_RATE 0.6
int rightHandPositionX[SEQ_NUM]={-1,-1,-1,-1,-1, -1,-1, -1, -1, -1};
int rightElbowPositionX[SEQ_NUM]={-1,-1,-1,-1,-1, -1,-1, -1, -1, -1};
DWORD WINAPI OnListeningHand(LPVOID lpParam);
void drawSkeleton(Mat &image, CvPoint pointSet[], int witchone); 
void getSkeletonImage(HANDLE &skeletonEvent, Mat &skeletonImage); 
CvPoint skeletonPoint[NUI_SKELETON_COUNT][NUI_SKELETON_POSITION_COUNT]={cvPoint(0,0)}; 


int main(int argc, char *argv[])
{
	Mat skeletonImage;
	skeletonImage.create(240, 320, CV_8UC3); 
	HRESULT hr = NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON);   
    if( hr != S_OK )   
    {   
        cout<<"NuiInitialize failed!"<<endl;   
		Sleep(1000);
        return -1;   
    } 
	HANDLE skeletonEvent = CreateEvent( NULL, TRUE, FALSE, NULL ); 
    hr = NuiSkeletonTrackingEnable( skeletonEvent, 0 ); 
    if( hr != S_OK )   
    {   
        cout << "NuiSkeletonTrackingEnable failed!" << endl;   
        NuiShutdown(); 
		Sleep(1000);
        return -1;   
    } 

	HANDLE handle=CreateThread(NULL, 0, OnListeningHand, NULL, 0, 0);
 	if(!handle)
 		cout<<"Creating OnListenHand thread failed!"<<endl;

	for(int i=0; i<SEQ_NUM; i++)
	{
		rightHandPositionX[i]=-1;
		rightElbowPositionX[i]=-1;
	}

	namedWindow("SkeletonImage", CV_WINDOW_AUTOSIZE);

	while (true) 
    {
		if(WaitForSingleObject(skeletonEvent, INFINITE)==0)
			getSkeletonImage(skeletonEvent, skeletonImage); 

		imshow("SkeletonImage", skeletonImage); 

		if(cvWaitKey(1)==27) 
            break; 
	}
	NuiShutdown(); 
	return 0; 
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
        cout << "Get the skeleton data failed!" << endl; 
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

//传入关节点位置画出人物骨架
void drawSkeleton(Mat &image, CvPoint pointSet[], int whichone) 
{ 
    CvScalar color; 
    switch(whichone) //跟踪不同的人显示不同的颜色 
    { 
    case 0: 
        color = cvScalar(255, 0, 0); 
        break; 
    case 1: 
        color = cvScalar(0,255, 0); 
        break; 
    case 2: 
        color = cvScalar(0, 0, 255); 
        break; 
    case 3: 
        color = cvScalar(255, 255, 0); 
        break; 
    case 4: 
        color = cvScalar(255, 0, 255); 
        break; 
    case 5: 
        color = cvScalar(0, 255, 255); 
        break; 
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


	//double deltaX=pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT].x-pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT].x;
	//double deltaY=pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT].y-pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT].y;
	//double gTheta=atan(deltaX/1.0/deltaY)/3.1415926*180;
	double gTheta=getAtan(pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT], pointSet[NUI_SKELETON_POSITION_WRIST_RIGHT]);
	cout<<gTheta<<endl;
	Sleep(1000);


	numSample++;
	if(numSample==SAMPLE_DELT)
	{
			rightHandPositionX[seqNum]=pointSet[NUI_SKELETON_POSITION_HAND_RIGHT].x;
			rightElbowPositionX[seqNum]=pointSet[NUI_SKELETON_POSITION_ELBOW_RIGHT].x;
			seqNum++;
			if(seqNum==SEQ_NUM)
				seqNum=0;

			numSample=0;
	}
} 

DWORD WINAPI OnListeningHand(LPVOID lpParam)
{
	while(true)
	{
		if((rightHandPositionX[SEQ_NUM-1]!=-1) && (rightElbowPositionX[SEQ_NUM-1]!=-1))
		{
			for(int i=0; i<SEQ_NUM-1; i++)
			{
				if((rightHandPositionX[i+1]-rightHandPositionX[i] < 0) && (rightElbowPositionX[i+1]-rightElbowPositionX[i] < 0))
					guessRate++;
			}
			if(guessRate > RECOGNIZE_RATE*SEQ_NUM)
				printf("got it!******\n");
			guessRate=0;
		}
		else
		{
			Sleep(1000);
		}
	}
	return 0;
}