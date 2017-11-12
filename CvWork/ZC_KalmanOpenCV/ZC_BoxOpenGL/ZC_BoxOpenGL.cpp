#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <gl/glaux.h>
#include "ZC_Socket.h"


#pragma comment(lib, "OPENGL32.LIB")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")

float ratX = 60;
float ratY = 60;
float ratZ = 60;


void onInitGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);				// 启用深度测试
}

void onChangeSize(GLint w, GLint h)
{
	if (h == 0)	h = 1;		
	glViewport(0, 0, w, h);															
	glMatrixMode(GL_PROJECTION);										
	glLoadIdentity();	
	gluPerspective(45.0, (GLdouble)w/(GLdouble)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();									
}

void onDrawBox()
{
	glBegin(GL_QUADS);
	// 前面
	glColor3f(1, 0, 0);
	glVertex3f(-1.0f, -1.0f,   1.0f);		// 四边形的左下
	glVertex3f( 1.0f, -1.0f,   1.0f);			// 四边形的右下
	glVertex3f( 1.0f,   1.0f,  1.0f);			// 四边形的右上
	glVertex3f(-1.0f,   1.0f,  1.0f);			// 四边形的左上
	// 后面
	glColor3f(0, 1, 0);
	glVertex3f(-1.0f, -1.0f, -1.0f);			// 四边形的右下
	glVertex3f(-1.0f,  1.0f, -1.0f);			// 四边形的右上
	glVertex3f( 1.0f,  1.0f, -1.0f);			// 四边形的左上
	glVertex3f( 1.0f, -1.0f, -1.0f);			// 四边形的左下
	// 顶面
	glColor3f(0, 0, 1);
	glVertex3f(-1.0f,  1.0f, -1.0f);			// 四边形的左上
	glVertex3f(-1.0f,  1.0f,  1.0f);			// 四边形的左下
	glVertex3f( 1.0f,  1.0f,  1.0f);			// 四边形的右下
	glVertex3f( 1.0f,  1.0f, -1.0f);			// 四边形的右上
	// 底面
	glColor3f(1, 1, 0);
	glVertex3f(-1.0f, -1.0f, -1.0f);			// 四边形的右上
	glVertex3f( 1.0f, -1.0f, -1.0f);			// 四边形的左上
	glVertex3f( 1.0f, -1.0f,  1.0f);			// 四边形的左下
	glVertex3f(-1.0f, -1.0f,  1.0f);			// 四边形的右下
	// 右面
	glColor3f(0, 1, 1);
	glVertex3f( 1.0f, -1.0f, -1.0f);			// 四边形的右下
	glVertex3f( 1.0f,  1.0f, -1.0f);			// 四边形的右上
	glVertex3f( 1.0f,  1.0f,  1.0f);			// 四边形的左上
	glVertex3f( 1.0f, -1.0f,  1.0f);			// 四边形的左下
	// 左面
	glColor3f(1, 0, 1);
	glVertex3f(-1.0f, -1.0f, -1.0f);			// 四边形的左下
	glVertex3f(-1.0f, -1.0f,  1.0f);			// 四边形的右下
	glVertex3f(-1.0f,  1.0f,  1.0f);			// 四边形的右上
	glVertex3f(-1.0f,  1.0f, -1.0f);			// 四边形的左上
	glEnd();
}

void onRenderScene(void)
{
	// 用glClearColor设置的颜色来清除屏幕
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(0, 0, -6.0f);

	// 绕x, y, z轴旋转
	glRotatef(ratX, 1.0f, 0.0f, 0.0f);
	glRotatef(ratY, 0.0f, 1.0f, 0.0f);
	glRotatef(ratZ, 0.0f, 0.0f, 1.0f);
	onDrawBox();

	glFlush();
	glutSwapBuffers();
}

void onKeyEvent(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ' ':
		ratX = 0;
		ratY = 0;
		ratZ = 0;
		break;
	}
}

void onTimerProc(int id)  
{  
	//std::cout<<"OnTimerProc##########"<<std::endl;
	glutTimerFunc(33, onTimerProc, 1); 
	glutPostRedisplay();
}  

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("RotateBox_OpenGL");

	glutDisplayFunc(onRenderScene);
	glutReshapeFunc(onChangeSize);
	glutKeyboardFunc(onKeyEvent);
	glutTimerFunc(33, onTimerProc, 1); 

	WSADATA wsadata;														//打开服务器监听客户端
	int sockRecv;
	if(WSAStartup(MAKEWORD(2, 2), &wsadata)!=0)
	{
		std::cout<<"WSAStartup 失败!"<<std::endl;
		return -1;
	}
	sockRecv=socket(AF_INET, SOCK_STREAM, 0);
	if(sockRecv==INVALID_SOCKET)
	{
		std::cout<<"SockRecv 初始化失败!"<<std::endl;
		return -1;
	}
	sockaddr_in sin;
	sin.sin_port=htons(PORT);
	sin.sin_family=AF_INET;
	sin.sin_addr.s_addr=0;
	if(bind(sockRecv, (sockaddr*)&sin, sizeof(sockaddr))==SOCKET_ERROR)
	{
		std::cout<<"Bind 失败!"<<std::endl;
		return -1;
	}
	listen(sockRecv, 50);
	HANDLE handle=CreateThread(NULL, 0, OnListingAccept, (void*)sockRecv, 0, 0);
	if(!handle)
		std::cout<<"创建 OnListingAccept 线程失败!"<<std::endl;
	CloseHandle(handle);

	//1.kalman filter setup
	kalman = cvCreateKalman( STATE_NUM, MEASURE_NUM, 0 );
	predictPosition = cvCreateMat( MEASURE_NUM, 1, CV_32FC1 );
	float TransitionMatrix[STATE_NUM][STATE_NUM] = {		//transition matrix
		1, 0, 0, 5, 0, 0,
		0, 1, 0, 0, 5, 0,
		0, 0, 1, 0, 0, 5,
		0, 0, 0, 1, 0, 0,
		0, 0, 0, 0, 1, 0,
		0, 0, 0, 0, 0, 1,
	};
	memcpy( kalman->transition_matrix->data.fl, TransitionMatrix, sizeof(TransitionMatrix));
	cvSetIdentity(kalman->measurement_matrix, cvRealScalar(1));
	cvSetIdentity(kalman->process_noise_cov, cvRealScalar(1e-5));
	cvSetIdentity(kalman->measurement_noise_cov, cvRealScalar(1e-1));
	cvSetIdentity(kalman->error_cov_post, cvRealScalar(1));
	CvRNG rng = cvRNG(-1);
	cvRandArr(&rng, kalman->state_post, CV_RAND_UNI, cvRealScalar(0), cvRealScalar(100));

	onInitGL();																				
	glutMainLoop();

	return 0;
}
