#include <WinSock2.h>
#include <iostream>


#define PORT 51112
#define RECV_BUFFER_SIZE 1024
char RecvBuffer[RECV_BUFFER_SIZE];
enum RECOGNIZE_RESULE 
{
	UP, DOWN, TOLEFT, TORIGHT, COMENEAR, GOFAR, COUNTERCLOCK, CLOCK, TOSTOP, NONE
};
int gRECOG_RESULT=NONE;

DWORD WINAPI OnListeningCommand(LPVOID lpParam)
{
	SOCKET sockRecv=(int)lpParam;
	recv(sockRecv, RecvBuffer, RECV_BUFFER_SIZE, 0);
	RecvBuffer[strlen(RecvBuffer)]='\0';
	std::cout<<RecvBuffer<<std::endl;

	if (!strcmp(RecvBuffer, "up"))
	{
		gRECOG_RESULT=UP;
	}
	else if (!strcmp(RecvBuffer, "down"))
	{
		gRECOG_RESULT=DOWN;
	}
	else if (!strcmp(RecvBuffer, "left"))
	{
		gRECOG_RESULT=TOLEFT;
	}
	else if (!strcmp(RecvBuffer, "right"))
	{
		gRECOG_RESULT=TORIGHT;
	}
	else if (!strcmp(RecvBuffer, "cclock"))
	{
		gRECOG_RESULT=COUNTERCLOCK;
	}
	else if (!strcmp(RecvBuffer, "clock"))
	{
		gRECOG_RESULT=CLOCK;
	}
	else if (!strcmp(RecvBuffer, "far"))
	{
		gRECOG_RESULT=GOFAR;
	}
	else if (!strcmp(RecvBuffer, "near"))
	{
		gRECOG_RESULT=COMENEAR;
	}
	else if (!strcmp(RecvBuffer, "stop"))
	{
		gRECOG_RESULT=TOSTOP;
	}

	memset(RecvBuffer, 0, sizeof(RecvBuffer));
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
		CreateThread(NULL, 0, OnListeningCommand, (void*)temp, 0, 0);
	}
	return 0;
}