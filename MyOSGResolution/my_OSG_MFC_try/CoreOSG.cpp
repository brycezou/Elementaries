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

MyManipulator* g_MyManipulator;

class MyManipulator : public osgGA::CameraManipulator
{
public:
	MyManipulator()
	{
		mPosition = osg::Vec3(0.0f, -100.0f, 100.0f);
		lastPos = mPosition;
		mRotation = osg::Vec3(osg::DegreesToRadians(80.0),0.0f,0.0f);
		step = 0.5;
		posOffset = osg::Vec3( 0.0, 0.0, 0.0 );
	}
	~MyManipulator()
	{
	}

public:
	virtual void setByMatrix(const osg::Matrixd& matrix)
	{
	}
	virtual void setByInverseMatrix(const osg::Matrixd& matrix)
	{
	}
	virtual osg::Matrix getMatrix(void)const
	{
		osg::Matrix mat;
		mat.makeRotate(mRotation._v[0], osg::Vec3(1.0f,0.0f,0.0f), mRotation._v[1],osg::Vec3(0.0f,1.0f,0.0f), mRotation._v[2],osg::Vec3(0.0f,0.0f,-1.0f));
		return mat*osg::Matrixd::translate(mPosition);
	}
	virtual osg::Matrix getInverseMatrix(void) const
	{
		osg::Matrix mat;
		mat.makeRotate(mRotation._v[0],osg::Vec3(1.0f,0.0f,0.0f), mRotation._v[1],osg::Vec3(0.0f,1.0f,0.0f), mRotation._v[2],osg::Vec3(0.0f,0.0f,-1.0f));
		return osg::Matrixd::inverse(mat*osg::Matrixd::translate(mPosition));
	}
	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us)
	{
		switch( ea.getEventType() )
		{
		case(osgGA::GUIEventAdapter::KEYDOWN):
			{
				switch(ea.getKey())
				{
				case( 'w' ):
					{
						posOffset.x() += step*sin(mRotation.z());
						posOffset.y() += step*cos(mRotation.z());
						return true;
					}
				case( 's' ):
					{
						posOffset.x() -= step*sin(mRotation.z());
						posOffset.y() -= step*cos(mRotation.z());
						return true;
					}
				case( 'a' ):
					{
						posOffset.x() -= step*cos( mRotation.z() );
						posOffset.y() += step*sin( mRotation.z() );
						return true;
					}
				case( 'd' ):
					{
						posOffset.x() += step*cos( mRotation.z() );
						posOffset.y() -= step*sin( mRotation.z() );
						return true;
					}

				case( ' ' ):
					{
						posOffset = osg::Vec3(0.0,0.0,0.0);
						mRotation = osg::Vec3(osg::PI/2.0,0.0,0.0);

						return true;
					}

				default:
					break;
				}
			}

		case( osgGA::GUIEventAdapter::PUSH ):
			{
				if( ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON )
				{
					pushPoint.x() = ea.getX();
					pushPoint.y() = ea.getY();
				}
				return true;
			}

		case( osgGA::GUIEventAdapter::DRAG ):
			{
				if( ea.getButtonMask() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON )
				{
					releasePoint.x() = ea.getX();
					releasePoint.y() = ea.getY();
					osg::Vec2 dir = releasePoint - pushPoint;
					mRotation.x() += dir.y()/1000.0;
					mRotation.z() += dir.x()/1000.0;

					pushPoint = releasePoint;
				}
				return true;
			}

		case( osgGA::GUIEventAdapter::SCROLL ):
			{
				if( ea.getScrollingMotion()==osgGA::GUIEventAdapter::SCROLL_UP )
				{
					posOffset.x() += step*sin(mRotation.z());
					posOffset.y() += step*cos(mRotation.z());
					posOffset.z() -= step*cos(mRotation.x());
				}
				else if( ea.getScrollingMotion()==osgGA::GUIEventAdapter::SCROLL_DOWN )
				{
					posOffset.x() -= step*sin(mRotation.z());
					posOffset.y() -= step*cos(mRotation.z());
					posOffset.z() += step*cos(mRotation.x());
				}

				return true;
			}

		default:
			break;
		}

		mPosition = lastPos + posOffset;

		return false;
	}

private:
	osg::Vec3 mPosition;				//视点
	osg::Vec3 posOffset;				//键盘造成的视点偏移
	osg::Vec3 mRotation;				//视角
	osg::Vec2 pushPoint;				
	osg::Vec2 releasePoint;
	float step;							//视点移动步长
	osg::Vec3 lastPos;					//最后一个视点位置
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

	osg::ref_ptr<osg::Node> cow = osgDB::readNodeFile( "cow.osg" );
	osg::ref_ptr<osg::Node> lz = osgDB::readNodeFile( "lz.osg" );

	osg::ref_ptr<osg::MatrixTransform> xLz = new osg::MatrixTransform;
	xLz->addChild( lz.get() );
	xLz->setMatrix( osg::Matrix::translate(osg::Vec3(0.0,0.0,-100.0)) );

	mRoot->addChild( cow.get() );
	mRoot->addChild( xLz.get() );

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
