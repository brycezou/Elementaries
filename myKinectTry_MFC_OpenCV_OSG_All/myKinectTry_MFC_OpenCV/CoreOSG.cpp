//------------------------------//------------------------------
//By FreeSouth at 2008 3 14 ieysx@163.com  www.osgChina.org 
//------------------------------//------------------------------

//
//OSG核心类实现文件
#include "stdafx.h"
#include "CoreOSG.h"

#include <osg/Notify>
#include <osg/TextureCubeMap>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>
#include <osg/Program>
#include <osg/CullFace>
#include <osg/Fog>
#include <osgText/Font>
#include <osg/Switch>
#include <osg/Texture3D>
#include <osg/MatrixTransform>
#include <osg/Image>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/CameraManipulator>
#include <osgGA/TrackballManipulator>
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIActionAdapter>
#include <osgGA/EventVisitor>

#include <osgText/Text>

#include <osgDB/ReadFile>

#include <string>
#include <vector>


extern enum RECOGNIZE_RESULE 
{
	UP, DOWN, TOLEFT, TORIGHT, COMENEAR, GOFAR, COUNTERCLOCK, CLOCK, TOSTOP, NONE
};
extern int gRECOG_RESULT;
enum WHICH_ACTION
{
	RUD, RDD, LEFTD, RIGHTD, NEARD, FARD, RLD, RRD, STOPD, NONED
};
int ACTION_DONE=NONED;
float gSumSpeed=0;
float gSumAngle=0;
#define DELTA_SPEED 0.05
#define DELTA_ANGLE 0.04

#define ML1 0x41			//move to the left
#define ML2 0x61
#define MR1 0x44			//move to the right
#define MR2 0x64
#define MF1 0x57			//move forward
#define MF2 0x77
#define MB1 0x53			//move backward
#define MB2 0x73
#define RR 0xFF53		//rotate to the right
#define RL 0xFF51		//rotate to the left
#define RU 0xFF52		//rotate upward
#define RD 0xFF54		//rotate downward

MyManipulator* g_MyManipulator;

class MyManipulator : public osgGA::CameraManipulator
{
private:
	float m_fMoveSpeed;
	osg::Vec3 m_vPosition;
	osg::Vec3 m_vRotation;
public:
	bool m_bLeftButtonDown;
	float m_fPushX;
	float m_fPushY;
	float m_fAngle;//屏幕角度

public:
	MyManipulator():	m_fMoveSpeed(10.0f),
								m_bLeftButtonDown(false),
								m_fPushX(0),
								m_fAngle(20),
								m_fPushY(0)
	{
		m_vPosition=osg::Vec3(0.0f, -120.0f, 100.0f);
		m_vRotation=osg::Vec3(osg::PI_2, 0.0f, 0.0f);
	}
	~MyManipulator()
	{
	}

public:
	void ChangePosition(osg::Vec3& delta)
	{
		m_vPosition+=delta;
	}
	virtual void setByMatrix(const osg::Matrixd& matrix)
	{
	}
	virtual void setByInverseMatrix(const osg::Matrixd& matrix)
	{
	}
	virtual osg::Matrix getMatrix(void) const
	{
		osg::Matrixd mat;
		mat.makeRotate(m_vRotation._v[0], osg::Vec3(1.0f, 0.0f, 0.0f), m_vRotation._v[1], osg::Vec3(0.0f, 1.0f, 0.0f), m_vRotation._v[2], osg::Vec3(0.0f, 0.0f, 1.0f));
		return mat*osg::Matrixd::translate(m_vPosition);
	}
	virtual osg::Matrix getInverseMatrix(void) const
	{
		osg::Matrixd mat;
		mat.makeRotate(m_vRotation._v[0], osg::Vec3(1.0f, 0.0f, 0.0f), m_vRotation._v[1], osg::Vec3(0.0f, 1.0f, 0.0f), m_vRotation._v[2], osg::Vec3(0.0f, 0.0f, 1.0f));
		return osg::Matrixd::inverse(mat * osg::Matrixd::translate(m_vPosition));
	}
	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us)
	{
		float mouseX=ea.getX();
		float mouseY=ea.getY();
	//	switch(ea.getEventType())
	//	{
	//	case (osgGA::GUIEventAdapter::KEYDOWN):
	//		{
	//			if (ea.getKey()==MF1||ea.getKey()==MF2)	//向前平移
	//			{
	//				ChangePosition(osg::Vec3(m_fMoveSpeed*cosf(osg::PI_2+m_vRotation._v[2]), 0, 0));
	//				ChangePosition(osg::Vec3(0, m_fMoveSpeed*sinf(osg::PI_2+m_vRotation._v[2]), 0));
	//				return true;
	//			}
	//			if (ea.getKey()==MB1||ea.getKey()==MB2)	//向后平移
	//			{
	//				ChangePosition(osg::Vec3(-m_fMoveSpeed*cosf(osg::PI_2+m_vRotation._v[2]), 0, 0));
	//				ChangePosition(osg::Vec3(0, -m_fMoveSpeed*sinf(osg::PI_2+m_vRotation._v[2]), 0));
	//				return true;
	//			}
	//			if (ea.getKey()==ML1||ea.getKey()==ML2)	//向左平移
	//			{
	//				ChangePosition(osg::Vec3(-m_fMoveSpeed*sinf(osg::PI_2+m_vRotation._v[2]), 0, 0));
	//				ChangePosition(osg::Vec3(0, m_fMoveSpeed*cosf(osg::PI_2+m_vRotation._v[2]), 0));
	//				return true;
	//			}
	//			if (ea.getKey()==MR1||ea.getKey()==MR2)	//向右平移
	//			{
	//				ChangePosition(osg::Vec3(m_fMoveSpeed*sinf(osg::PI_2+m_vRotation._v[2]), 0, 0));
	//				ChangePosition(osg::Vec3(0, -m_fMoveSpeed*cosf(osg::PI_2+m_vRotation._v[2]), 0));
	//				return true;
	//			}

	//			if (ea.getKey()==RR)	//向右旋转
	//			{
	//				m_vRotation._v[2]-=osg::DegreesToRadians(m_fAngle);
	//			}
	//			if (ea.getKey()==RL)	//向左旋转
	//			{
	//				m_vRotation._v[2]+=osg::DegreesToRadians(m_fAngle);
	//			}
	//			if (ea.getKey()==RU)	//向上旋转
	//			{
	//				m_vRotation._v[0]+=osg::DegreesToRadians(0.5*m_fAngle);
	//				if (m_vRotation._v[0]>=osg::PI*7/12)
	//				{
	//					m_vRotation._v[0]=osg::PI*7/12;
	//				}
	//				if (m_vRotation._v[0]<=osg::PI*5/12)	//between 1/3 and 1/2 pi
	//				{
	//					m_vRotation._v[0]=osg::PI*5/12;
	//				}
	//			}
	//			if (ea.getKey()==RD)	//向下旋转
	//			{
	//				m_vRotation._v[0]-=osg::DegreesToRadians(0.5*m_fAngle);
	//				if (m_vRotation._v[0]>=osg::PI*7/12)
	//				{
	//					m_vRotation._v[0]=osg::PI*7/12;
	//				}
	//				if (m_vRotation._v[0]<=osg::PI*5/12)	//between 1/3 and 1/2 pi
	//				{
	//					m_vRotation._v[0]=osg::PI*5/12;
	//				}
	//			}

	//			return false;
	//		}

	//		//鼠标按下
	//	case (osgGA::GUIEventAdapter::PUSH):
	//		if (ea.getButton()==1)
	//		{
	//			m_fPushX=mouseX;
	//			m_fPushY=mouseY;
	//			m_bLeftButtonDown=true;
	//		}
	//		return false;

	//		//拖动
	//	case (osgGA::GUIEventAdapter::DRAG):
	//		if (m_bLeftButtonDown)
	//		{
	//			m_vRotation._v[2]-=osg::DegreesToRadians(0.05*m_fAngle*(mouseX-m_fPushX));
	//			m_vRotation._v[0]+=osg::DegreesToRadians(0.05*m_fAngle*(mouseY-m_fPushY));
	//			if (m_vRotation._v[0]>=osg::PI*7/12)
	//			{
	//				m_vRotation._v[0]=osg::PI*7/12;
	//			}
	//			if (m_vRotation._v[0]<=osg::PI*5/12)	//between 1/3 and 1/2 pi
	//			{
	//				m_vRotation._v[0]=osg::PI*5/12;
	//			}
	//			m_fPushX=mouseX;
	//			m_fPushY=mouseY;
	//		}
	//		return false;

	//		//鼠标释放
	//	case (osgGA::GUIEventAdapter::RELEASE):
	//		if (ea.getButton()==1)
	//		{
	//			m_bLeftButtonDown=false;
	//		}
	//		return false;

	//	//default:
	//	//	return false;
	//	}

	//	switch(gRECOG_RESULT)
	//	{
	//	case UP:
	//		ChangePosition(osg::Vec3(-m_fMoveSpeed*cosf(osg::PI_2+m_vRotation._v[2]), 0, 0));
	//		ChangePosition(osg::Vec3(0, -m_fMoveSpeed*sinf(osg::PI_2+m_vRotation._v[2]), 0));
	//		gRECOG_RESULT=NONE;
	//		return true;
	//	case DOWN:
	//		ChangePosition(osg::Vec3(m_fMoveSpeed*cosf(osg::PI_2+m_vRotation._v[2]), 0, 0));
	//		ChangePosition(osg::Vec3(0, m_fMoveSpeed*sinf(osg::PI_2+m_vRotation._v[2]), 0));
	//		gRECOG_RESULT=NONE;
	//		return true;
	//	case LEFT:
	//		ChangePosition(osg::Vec3(-m_fMoveSpeed*sinf(osg::PI_2+m_vRotation._v[2]), 0, 0));
	//		ChangePosition(osg::Vec3(0, m_fMoveSpeed*cosf(osg::PI_2+m_vRotation._v[2]), 0));
	//		gRECOG_RESULT=NONE;
	//		return true;
	//	case RIGHT:
	//		ChangePosition(osg::Vec3(m_fMoveSpeed*sinf(osg::PI_2+m_vRotation._v[2]), 0, 0));
	//		ChangePosition(osg::Vec3(0, -m_fMoveSpeed*cosf(osg::PI_2+m_vRotation._v[2]), 0));
	//		gRECOG_RESULT=NONE;
	//		return true;
	//	case COUNTERCLOCK:
	//		m_vRotation._v[2]+=osg::DegreesToRadians(m_fAngle);
	//		gRECOG_RESULT=NONE;
	//		return true;
	//	case CLOCK:
	//		m_vRotation._v[2]-=osg::DegreesToRadians(m_fAngle);
	//		gRECOG_RESULT=NONE;
	//		return true;
	//	case NONE:
	//		break;
	//	}
	//	return false;
	//}

		switch(ea.getEventType())
		{
		case (osgGA::GUIEventAdapter::KEYDOWN):		//按键事件
			{
				if (ea.getKey()==MF1||ea.getKey()==MF2)	//向前平移
				{
					ACTION_DONE=FARD;
					gSumSpeed=0;
					break;
				}
				else if (ea.getKey()==MB1||ea.getKey()==MB2)	//向后平移
				{
					ACTION_DONE=NEARD;
					gSumSpeed=0;
					break;
				}
				else if (ea.getKey()==ML1||ea.getKey()==ML2)		//向左平移
				{
					ACTION_DONE=LEFTD;
					gSumSpeed=0;
					break;
				}
				else if (ea.getKey()==MR1||ea.getKey()==MR2)	//向右平移
				{
					ACTION_DONE=RIGHTD;
					gSumSpeed=0;
					break;
				}
				else if (ea.getKey()==RR)	//向右旋转
				{
					ACTION_DONE=RRD;
					gSumAngle=0;
					break;
				}
				else if (ea.getKey()==RU)	//向上旋转
				{
					ACTION_DONE=RUD;
					gSumAngle=0;
					break;
				}
				else if (ea.getKey()==RD)	//向下旋转
				{
					ACTION_DONE=RDD;
					gSumAngle=0;
					break;
				}
				else if (ea.getKey()==RL)	//向左旋转
				{
					ACTION_DONE=RLD;
					gSumAngle=0;
					break;
				}
			}
		case (osgGA::GUIEventAdapter::PUSH):	//鼠标按下事件
			if (ea.getButton()==1)
			{
				m_fPushX=mouseX;
				m_fPushY=mouseY;
				m_bLeftButtonDown=true;
			}
			return false;

		case (osgGA::GUIEventAdapter::DRAG):		//鼠标拖动事件
			if (m_bLeftButtonDown)
			{
				m_vRotation._v[2]-=osg::DegreesToRadians(DELTA_SPEED*m_fAngle*(mouseX-m_fPushX));
				m_vRotation._v[0]+=osg::DegreesToRadians(DELTA_SPEED*m_fAngle*(mouseY-m_fPushY));
				if (m_vRotation._v[0]>=osg::PI*7/12)
				{
					m_vRotation._v[0]=osg::PI*7/12;
				}
				if (m_vRotation._v[0]<=osg::PI*5/12)	//between 1/3 and 1/2 pi
				{
					m_vRotation._v[0]=osg::PI*5/12;
				}
				m_fPushX=mouseX;
				m_fPushY=mouseY;
			}
			return false;

		case (osgGA::GUIEventAdapter::RELEASE):			//鼠标释放事件
			if (ea.getButton()==1)
			{
				m_bLeftButtonDown=false;
			}
			return false;
		}

		switch(gRECOG_RESULT)
		{
		case UP:
			gRECOG_RESULT=NONE;
			ACTION_DONE=RUD;
			gSumSpeed=0;
			break;
		case DOWN:
			gRECOG_RESULT=NONE;
			ACTION_DONE=RDD;
			gSumSpeed=0;
			break;
		case TOLEFT:
			gRECOG_RESULT=NONE;
			ACTION_DONE=LEFTD;
			gSumSpeed=0;
			break;
		case TORIGHT:
			gRECOG_RESULT=NONE;
			ACTION_DONE=RIGHTD;
			gSumSpeed=0;
			break;
		case COUNTERCLOCK:
			gRECOG_RESULT=NONE;
			ACTION_DONE=RLD;
			gSumAngle=0;
			break;
		case CLOCK:
			gRECOG_RESULT=NONE;
			ACTION_DONE=RRD;
			gSumAngle=0;
			break;
		case COMENEAR:
			gRECOG_RESULT=NONE;
			ACTION_DONE=NEARD;
			gSumAngle=0;
			break;
		case GOFAR:
			gRECOG_RESULT=NONE;
			ACTION_DONE=FARD;
			gSumAngle=0;
			break;
		case TOSTOP:
			gRECOG_RESULT=NONE;
			ACTION_DONE=STOPD;
			gSumAngle=0;
			gSumSpeed=0;
			break;
		case NONE:
			break;
		}
		
		switch(ACTION_DONE)
		{
		case FARD:
			ChangePosition(osg::Vec3(DELTA_SPEED*cosf(osg::PI_2+m_vRotation._v[2]), 0, 0));
			ChangePosition(osg::Vec3(0, DELTA_SPEED*sinf(osg::PI_2+m_vRotation._v[2]), 0));
			gSumSpeed+=DELTA_SPEED;
			if (gSumSpeed>m_fMoveSpeed)
			{
				gSumSpeed=0;
				ACTION_DONE=NONED;
			}
			return true;
		case NEARD:
			ChangePosition(osg::Vec3(-DELTA_SPEED*cosf(osg::PI_2+m_vRotation._v[2]), 0, 0));
			ChangePosition(osg::Vec3(0, -DELTA_SPEED*sinf(osg::PI_2+m_vRotation._v[2]), 0));
			gSumSpeed+=DELTA_SPEED;
			if (gSumSpeed>m_fMoveSpeed)
			{
				gSumSpeed=0;
				ACTION_DONE=NONED;
			}
			return true;
		case LEFTD:
			ChangePosition(osg::Vec3(-DELTA_SPEED*sinf(osg::PI_2+m_vRotation._v[2]), 0, 0));
			ChangePosition(osg::Vec3(0, DELTA_SPEED*cosf(osg::PI_2+m_vRotation._v[2]), 0));
			gSumSpeed+=DELTA_SPEED;
			if (gSumSpeed>m_fMoveSpeed)
			{
				gSumSpeed=0;
				ACTION_DONE=NONED;
			}
			return true;
		case RIGHTD:
			ChangePosition(osg::Vec3(DELTA_SPEED*sinf(osg::PI_2+m_vRotation._v[2]), 0, 0));
			ChangePosition(osg::Vec3(0, -DELTA_SPEED*cosf(osg::PI_2+m_vRotation._v[2]), 0));
			gSumSpeed+=DELTA_SPEED;
			if (gSumSpeed>m_fMoveSpeed)
			{
				gSumSpeed=0;
				ACTION_DONE=NONED;
			}
			return true;
		case RRD:
			m_vRotation._v[2]-=osg::DegreesToRadians(DELTA_ANGLE);
			gSumAngle+=DELTA_ANGLE;
			if (gSumAngle>m_fAngle)
			{
				gSumAngle=0;
				ACTION_DONE=NONED;
			}
			return false;
		case RLD:
			m_vRotation._v[2]+=osg::DegreesToRadians(DELTA_ANGLE);
			gSumAngle+=DELTA_ANGLE;
			if (gSumAngle>m_fAngle)
			{
				gSumAngle=0;
				ACTION_DONE=NONED;
			}
			return false;
		case RUD:
			m_vRotation._v[0]+=osg::DegreesToRadians(DELTA_ANGLE);
			gSumAngle+=DELTA_ANGLE;
			if (gSumAngle>m_fAngle/3.0)
			{
				gSumAngle=0;
				ACTION_DONE=NONED;
			}
			if (m_vRotation._v[0]>=osg::PI*7/12)
			{
				m_vRotation._v[0]=osg::PI*7/12;
			}
			if (m_vRotation._v[0]<=osg::PI*5/12)	//between 1/3 and 1/2 pi
			{
				m_vRotation._v[0]=osg::PI*5/12;
			}
			return false;
		case RDD:
			m_vRotation._v[0]-=osg::DegreesToRadians(DELTA_ANGLE);
			gSumAngle+=DELTA_ANGLE;
			if (gSumAngle>m_fAngle/3.0)
			{
				gSumAngle=0;
				ACTION_DONE=NONED;
			}
			if (m_vRotation._v[0]>=osg::PI*7/12)
			{
				m_vRotation._v[0]=osg::PI*7/12;
			}
			if (m_vRotation._v[0]<=osg::PI*5/12)	//between 1/3 and 1/2 pi
			{
				m_vRotation._v[0]=osg::PI*5/12;
			}
			return false;
		case STOPD:
		case NONED:
			ACTION_DONE=NONED;
			gSumAngle=0;
			gSumSpeed=0;
			return false;
		}

		return false;
	}	
};


CCoreOSG::CCoreOSG(HWND hWnd) :
   m_hWnd(hWnd) 
{

}

CCoreOSG::~CCoreOSG()
{
	//析构操作
    mViewer->setDone(true);
    Sleep(1000);
    mViewer->stopThreading();

    delete mViewer;
}

//OSG的初始化，包括场景，漫游器，视口的初始化。
void CCoreOSG::InitOSG()
{
    InitSceneGraph();
	InitManipulators();
    InitCameraConfig();
}

void CCoreOSG::InitSceneGraph(void)
{
    // 初始化组
    mRoot  = new osg::Group;
	osg::ref_ptr<osg::Node> lz = osgDB::readNodeFile( "lz.osg" );
	mRoot->addChild( lz.get() );
}

void CCoreOSG::InitManipulators(void)
{
	g_MyManipulator = new MyManipulator();
}

void CCoreOSG::InitCameraConfig(void)
{
    // 局部变量存放窗口矩形
    RECT rect;

    // 创建一个viewer
    mViewer = new osgViewer::Viewer();
    
    // 得到当前窗口矩形
    ::GetWindowRect(m_hWnd, &rect);

    // 初始化图形描述什么东西
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;

    // 初始化窗口变量，为OSG所用
    osg::ref_ptr<osg::Referenced> windata = new osgViewer::GraphicsWindowWin32::WindowData(m_hWnd);

    // 设置一些个参数
    traits->x = 0;
    traits->y = 0;
    traits->width = rect.right - rect.left;
    traits->height = rect.bottom - rect.top;
	//traits->width = 1000;
	//traits->height = 800;
    traits->windowDecoration = true;
    traits->doubleBuffer = true;
    traits->sharedContext = 0;
	//以下两行决定是否新开窗口
    traits->setInheritedWindowPixelFormat = true;
    traits->inheritedWindowData = windata;

    // 创建图形上下文
    osg::GraphicsContext* gc = osg::GraphicsContext::createGraphicsContext(traits.get());

	//宽屏不变形_主视图
	double fovy ,aspectRation ,zNear ,zFar;
	mViewer->getCamera()->getProjectionMatrixAsPerspective(fovy,aspectRation,zNear,zFar);
	double newAspectRatio = double(traits->width)/double(traits->height);
	double aspectRatioChange = newAspectRatio/aspectRation;
	if(aspectRatioChange != 1.0)
	{
		mViewer->getCamera()->getProjectionMatrix() *= osg::Matrix::scale(1.0/aspectRatioChange,1.0,1.0);
	}

	mViewer->getCamera()->setViewport(new osg::Viewport(0,0,traits->width,traits->height));

    // 绑
    mViewer->getCamera()->setGraphicsContext(gc);

    // 添加操作器到VIEWER
    mViewer->setCameraManipulator( g_MyManipulator );

    //设置数据
    mViewer->setSceneData(mRoot.get());

    // 实现VIEWER
    mViewer->realize();
}

void CCoreOSG::PreFrameUpdate()
{
    // 帧前操作可在这里做一下
}

void CCoreOSG::PostFrameUpdate()
{
    // 帧后操作可在这里做一下
}

void CCoreOSG::Render(void* ptr)
{
	CCoreOSG* osg = (CCoreOSG*)ptr;
    osgViewer::Viewer* viewer = osg->getViewer();

    while(!viewer->done())
    {
        osg->PreFrameUpdate();
        viewer->frame();
        osg->PostFrameUpdate();
    }

    _endthread();
}

void CCoreOSG::ReplaceSceneData(std::string filename)
{
	//移掉mRoot中所有的数据,这些数据将会被自动释放掉，不用手动搞它
	mRoot ->removeChildren(0, mRoot ->getNumChildren()) ;
   
	//加入要置换的模型
	mModel = osgDB::readNodeFile(filename);

	//优化
    osgUtil::Optimizer optimizer;
    optimizer.optimize(mModel.get());
    optimizer.reset();

	//添加孩子
    mRoot->addChild(mModel.get());
}
