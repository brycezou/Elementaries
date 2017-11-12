#include <Windows.h>
#include <iostream>
#include <time.h>
#include "glut.h"

using namespace std;


#define 	BRICK_STEP						2
#define 	BIRD_RADIUS					30
#define 	BIRD_UP_STEP					35
#define 	BRICK_WIDTH					60
#define 	BRICK_EMPTY					240
#define 	BRICK_DISTANCE			120
#define 	TIMER_INTERVAL			25
#define 	SCREEN_WIDTH				640
#define 	SCREEN_HEIGHT				480
#define		MAX_CHAR						128
#define		PORT									55555
#define		RECV_BUFFER_SIZE		16

typedef struct Brick
{
	int x;
	int y;
	int state;		//if scored then 1 or 0
	Brick *next;
}Brick;

Brick  *m_brickList;
int			m_birdY;
int			m_Counter;
int			m_Score;
bool		m_IsUsingTimer = false;
char		RecvBuffer[RECV_BUFFER_SIZE];
DWORD WINAPI OnListingAccept(LPVOID lpParam);


int getRandom(void)
{
	srand(time(NULL)*time(NULL));
	int randInt = 20+rand()%(SCREEN_HEIGHT-20*2-BRICK_EMPTY);
	return randInt;
}

void drawText(int x, int y, char* str) 
{
	static bool isFirstCall = true;
	static GLuint lists;
	//if first call, initial a list for each ASCII char
	if( isFirstCall )
	{ 
		isFirstCall = false;
		//malloc a MAX_CHAR length display number array
		lists = glGenLists(MAX_CHAR);
		//just fos windows OS
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}

	int nx = x - SCREEN_WIDTH/2;
	int ny = -y + SCREEN_HEIGHT/2;
	glColor3f(1.0f, 0.0f, 0.0f);
	glRasterPos2f(nx, ny);
	while(*str != '\0')
	{
		glCallList(lists + *str);
		str++;
	}
}

void OnInitialEntity(void)
{
	m_brickList = new Brick[1];
	m_brickList->x = SCREEN_WIDTH;
	m_brickList->y = getRandom();
	m_brickList->state = 0;
	m_brickList->next = NULL;

	m_birdY = SCREEN_HEIGHT/2;
	m_Counter = 0;
	m_Score = 0;
}

void OnInitialGL(void)
{
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	OnInitialEntity();
}

void OnSafeInitial(void)
{
	Brick *pbrick = m_brickList->next;
	while(pbrick != NULL)
	{
		Brick *pTemp = pbrick;
		pbrick = pbrick->next;
		delete []pTemp;
	}
	m_brickList->x = SCREEN_WIDTH;
	m_brickList->y = getRandom();
	m_brickList->state = 0;
	m_brickList->next = NULL;

	m_birdY = SCREEN_HEIGHT/2;
	m_Counter = 0;
	m_Score = 0;
}

int OnInitialSocket(void)
{
	WSADATA wsadata;
	if(WSAStartup(MAKEWORD(2, 2), &wsadata)!=0)
	{
		cout<<"WSAStartup failed"<<endl;
		return -1;
	}

	int sockRecv = socket(AF_INET, SOCK_STREAM, 0);
	if(sockRecv == INVALID_SOCKET)
	{
		cout<<"sockRecv initial failed"<<endl;
		return -1;
	}

	sockaddr_in sin;
	sin.sin_port = htons(PORT);
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = 0;

	if(bind(sockRecv, (sockaddr*)&sin, sizeof(sockaddr)) == SOCKET_ERROR)
	{
		cout<<"bind faild"<<endl;
		return -1;
	}

	listen(sockRecv, 5);

	HANDLE handle = CreateThread(NULL, 0, OnListingAccept, (void*)sockRecv, 0, 0);
	if(!handle)
	{
		cout<<"failed to create OnListingAccept thread"<<endl;
		return -1;
	}
	CloseHandle(handle);

	return 0;
}

void drawBackground(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
		glVertex2f(-SCREEN_WIDTH/2+1, SCREEN_HEIGHT/2-1);
		glVertex2f(-SCREEN_WIDTH/2+1, -SCREEN_HEIGHT/2);
		glVertex2f(SCREEN_WIDTH/2, -SCREEN_HEIGHT/2);
		glVertex2f(SCREEN_WIDTH/2, SCREEN_HEIGHT/2-1);
	glEnd();
}

void drawRect(int x, int y, int width, int height)
{
	int nx = x - SCREEN_WIDTH/2;
	int ny = -y + SCREEN_HEIGHT/2;
	glRectf(nx, ny, nx+width, ny-height);
}

void createBricks(void)
{
	if(m_brickList->next == NULL)
	{
		if(m_brickList->x+BRICK_WIDTH >= 0)
			m_brickList->x -= BRICK_STEP;
		if(m_brickList->x+BRICK_WIDTH+BRICK_DISTANCE <= SCREEN_WIDTH)
		{
			m_brickList->next = new Brick[1];
			m_brickList->next->x = m_brickList->x+BRICK_WIDTH+BRICK_DISTANCE;
			m_brickList->next->y = getRandom();
			m_brickList->next->state = 0;
			m_brickList->next->next = NULL;
		}
	}
	else
	{
		if(m_brickList->x+BRICK_WIDTH >= 0)
			m_brickList->x -= BRICK_STEP;
		else
		{
			Brick *pTemp = m_brickList;
			m_brickList = m_brickList->next;
			delete []pTemp;
			m_brickList->x -= BRICK_STEP;
		}

		Brick *pbrick = m_brickList;
		while(pbrick->next != NULL)
		{
			pbrick = pbrick->next;
			pbrick->x -= BRICK_STEP;
		}

		if(pbrick->x+BRICK_WIDTH+BRICK_DISTANCE <= SCREEN_WIDTH)
		{
			pbrick->next = new Brick[1];
			pbrick->next->x = pbrick->x+BRICK_WIDTH+BRICK_DISTANCE;
			pbrick->next->y = getRandom();
			pbrick->next->state = 0;
			pbrick->next->next = NULL;
		}
	}
}

void drawBrick(int x, int y)
{
	glColor3f(0.0f, 1.0f, 0.0f);
	drawRect(x, 0, BRICK_WIDTH, SCREEN_HEIGHT);
	glColor3f(0.0f, 0.0f, 1.0f);
	drawRect(x, y, BRICK_WIDTH, BRICK_EMPTY);
}

void drawCircle(int x, int y, int diameter)
{
	int radius = diameter / 2;
	int nx = x - SCREEN_WIDTH/2 + radius;
	int ny = -y + SCREEN_HEIGHT/2 - radius;
	GLfloat TWOPI = 2.0f *3.14159f;
	glBegin(GL_TRIANGLE_FAN);
	for(int i = 0; i < 32; i++)
	{
		glVertex2f(nx+radius*cos(i*TWOPI/32), ny+radius*sin(i*TWOPI/32));
	}
	glEnd();
}

void drawBird(int x, int y)
{
	glColor3f(1.0f, 1.0f, 0.0f);
	drawCircle(x, y, BIRD_RADIUS);
}

void drawFinalBird(int x, int y)
{
	glColor3f(1.0f, 0.0f, 0.0f);
	drawCircle(x, y, BIRD_RADIUS);
}

bool detectCollision(int birdx, int birdy, int brickx, int bricky)
{
	bool b1 = (birdy < bricky) || (birdy+BIRD_RADIUS > bricky+BRICK_EMPTY);
	bool b2 = (birdx+BIRD_RADIUS > brickx) && (birdx < brickx+BRICK_WIDTH);
	bool bResult = b1 && b2;
	return bResult;
}

void dealWithFinish(void)
{
	while(m_birdY <= SCREEN_HEIGHT-BIRD_RADIUS)
	{
		drawBackground();
		Brick *pbrick = m_brickList;
		while(pbrick != NULL)
		{
			drawBrick(pbrick->x, pbrick->y);
			pbrick = pbrick->next;
		}
		drawFinalBird(160, m_birdY);
		m_birdY += 2;
	}
	drawFinalBird(160, SCREEN_HEIGHT-BIRD_RADIUS+1);

	Brick *pbrick = m_brickList->next;
	while(pbrick != NULL)
	{
		Brick *pTemp = pbrick;
		pbrick = pbrick->next;
		delete []pTemp;
	}
	m_brickList->next = NULL;
}

void showGrades(void)
{
	char strGrades[5];
	sprintf(strGrades, "%d", m_Score);
	drawText(50, 70, strGrades);
}

void OnSizeChanged(GLsizei w, GLsizei h)
{
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho (-SCREEN_WIDTH/2, SCREEN_WIDTH/2, -SCREEN_HEIGHT/2, SCREEN_HEIGHT/2, 1.0, -1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void OnRenderScene(void)
{
	bool IsFinished = false;
	drawBackground();
	Brick *pbrick = m_brickList;
	while(pbrick != NULL)
	{
		drawBrick(pbrick->x, pbrick->y);
		if(pbrick->state == 0 && pbrick->x+BRICK_WIDTH/2 <= 160)
		{
			pbrick->state = 1;
			m_Score++;
		}
		if(detectCollision(160, m_birdY, pbrick->x, pbrick->y))
		{
			m_IsUsingTimer = false;
			IsFinished = true;
		}
		pbrick = pbrick->next;
	}
	drawBird(160, m_birdY);
	if(IsFinished)
	{
		dealWithFinish();
		drawText(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, "GAME OVER!");
	}
	showGrades();
	glFlush();
}

void OnTimerProc(int id)  
{  
	if(m_IsUsingTimer)
	{
		createBricks();
		m_Counter++;
		if(m_birdY <= SCREEN_HEIGHT-BIRD_RADIUS-m_Counter)
			m_birdY += m_Counter;
		else
			m_birdY = SCREEN_HEIGHT-BIRD_RADIUS;
		OnRenderScene();
	}

	glutTimerFunc(25, OnTimerProc, 1); 
}  

void OnKeyEvent(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r'	:
	case 'R'	:
		m_IsUsingTimer = false;
		OnSafeInitial();
		m_IsUsingTimer = true;
		break;
	case 'w':
	case 'W':
		m_Counter = -8;
		if(m_birdY >= BIRD_UP_STEP)
			m_birdY -= BIRD_UP_STEP;
		else
			m_birdY = 0;
		break;
	case 'p':
	case 'P':
		m_IsUsingTimer = false;
		break;
	case 'b':
	case 'B':
		m_IsUsingTimer = true;
		break;
	}
}

DWORD WINAPI OnListeningCommand(LPVOID lpParam)
{
	SOCKET sockRecv=(int)lpParam;
	memset(RecvBuffer, 0, sizeof(char)*RECV_BUFFER_SIZE);
	while (true)
	{
		recv(sockRecv, RecvBuffer, RECV_BUFFER_SIZE, 0);
		RecvBuffer[strlen(RecvBuffer)]='\0';
		if(strcmp(RecvBuffer, "up") == 0)
		{
			m_Counter = -8;
			if(m_birdY >= BIRD_UP_STEP)
				m_birdY -= BIRD_UP_STEP;
			else
				m_birdY = 0;
		}
		else if (strcmp(RecvBuffer, "restart") == 0)
		{
			m_IsUsingTimer = false;
			OnSafeInitial();
			m_IsUsingTimer = true;
		}
		else if (strcmp(RecvBuffer, "begin") == 0)
		{
			m_IsUsingTimer = true;
		}
		else if (strcmp(RecvBuffer, "pause") == 0)
		{
			m_IsUsingTimer = false;
		}
		memset(RecvBuffer, 0, sizeof(char)*RECV_BUFFER_SIZE);
	}

	return 0;
}

DWORD WINAPI OnListingAccept(LPVOID lpParam)
{
	SOCKET temp, socket;
	socket=(int)lpParam;
	cout<<"listening..."<<endl;
	while(true)
	{
		temp=accept(socket, NULL, NULL);
		cout<<"one connected!"<<endl;
		CreateThread(NULL, 0, OnListeningCommand, (void*)temp, 0, 0);
	}
	return 0;
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("FlappyBird");

	glutDisplayFunc(OnRenderScene);
	glutReshapeFunc(OnSizeChanged);
	glutKeyboardFunc(OnKeyEvent);
	glutTimerFunc(25, OnTimerProc, 1); 

	if(OnInitialSocket() == -1)
	{
		cout<<"keyboard available only"<<endl;
	}
	OnInitialGL();																				
	glutMainLoop();

	return 0;
}
