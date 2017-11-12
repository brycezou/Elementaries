#include <WinSock.h>
#include <iostream>
#include "json.h"
#include <highgui.h>
#include <cv.h>


#define STATE_NUM			6		//x, y, z, dx, dy, dz
#define MEASURE_NUM	3		//x, y, z

#define PORT 51413
#define RECV_BUFFER_SIZE 1024
char RecvBuffer[RECV_BUFFER_SIZE];


CvKalman* kalman = 0;
CvMat* predictPosition = 0;
const CvMat* prediction = 0;
CvPoint3D32f realPoint = cvPoint3D32f(0, 0, 0);

extern float ratX;
extern float ratY;
extern float ratZ;


void onGetDataFor3D(char *strOneJson)
{
	std::cout<<strOneJson<<std::endl;
	json_value *val=json_parse(strOneJson);		
	//ratX = val->u.object.values[0].value->u.dbl;
	//ratY = val->u.object.values[1].value->u.dbl;
	//ratZ = val->u.object.values[2].value->u.dbl;

	realPoint = cvPoint3D32f(val->u.object.values[0].value->u.dbl, val->u.object.values[1].value->u.dbl, val->u.object.values[2].value->u.dbl);   
	prediction = cvKalmanPredict(kalman, 0);
	ratX = prediction->data.fl[0];
	ratY = prediction->data.fl[1];
	ratZ = prediction->data.fl[2];		//x, y, z

	//3.update predictPosition
	predictPosition->data.fl[0] = realPoint.x;
	predictPosition->data.fl[1] = realPoint.y;
	predictPosition->data.fl[2] = realPoint.z;
	//4.update
	cvKalmanCorrect(kalman, predictPosition);		
}

DWORD WINAPI OnListeningCommand(LPVOID lpParam)
{
	SOCKET sockRecv=(int)lpParam;
	char strOneJson[50];
	while (true)
	{
		recv(sockRecv, RecvBuffer, RECV_BUFFER_SIZE, 0);
		RecvBuffer[strlen(RecvBuffer)]='\0';

		int i = 0;
		while (RecvBuffer[i] == '{')
		{
			int j = 0;
			while(RecvBuffer[i] != '}' && RecvBuffer[i] != '\0') 
			{
				strOneJson[j] = RecvBuffer[i];
				i++;
				j++;
			}
			if (RecvBuffer[i] == '}')
			{
				strOneJson[j] = '}';
				strOneJson[j+1] = '\0';
				onGetDataFor3D(strOneJson);
				i += 2;
			}
		}

		while(RecvBuffer[i] != '{' && RecvBuffer[i] != '\0')   i++;
		while(RecvBuffer[i] == '{')
		{
			int j = 0;
			while(RecvBuffer[i] != '}' && RecvBuffer[i] != '\0') 
			{
				strOneJson[j] = RecvBuffer[i];
				i++;
				j++;
			}
			if (RecvBuffer[i] == '}')
			{
				strOneJson[j] = '}';
				strOneJson[j+1] = '\0';
				onGetDataFor3D(strOneJson);
				i += 2;
			}
		}

		memset(RecvBuffer, 0, sizeof(RecvBuffer));
	}

	return 0;
}

DWORD WINAPI OnListingAccept(LPVOID lpParam)
{
	SOCKET temp, socket;
	socket=(int)lpParam;
	std::cout<<"listening..."<<std::endl;
	while(true)
	{
		temp=accept(socket, NULL, NULL);
		std::cout<<"Client connected!"<<std::endl;
		CreateThread(NULL, 0, OnListeningCommand, (void*)temp, 0, 0);
	}
	return 0;
}