//------------------------------//------------------------------
//By FreeSouth at 2008 3 14 ieysx@163.com  www.osgChina.org 
//------------------------------//------------------------------

//
//OSG������ʵ���ļ�
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
	osg::Vec3 mPosition;				//�ӵ�
	osg::Vec3 posOffset;				//������ɵ��ӵ�ƫ��
	osg::Vec3 mRotation;				//�ӽ�
	osg::Vec2 pushPoint;				
	osg::Vec2 releasePoint;
	float step;							//�ӵ��ƶ�����
	osg::Vec3 lastPos;					//���һ���ӵ�λ��
};


CCoreOSG::CCoreOSG(HWND hWnd) :
   m_hWnd(hWnd) 
{

}

CCoreOSG::~CCoreOSG()
{
	//��������
    mViewer->setDone(true);
    Sleep(1000);
    mViewer->stopThreading();

    delete mViewer;
}

//OSG�ĳ�ʼ�����������������������ӿڵĳ�ʼ����
void CCoreOSG::InitOSG()
{
    InitSceneGraph();
	InitManipulators();
    InitCameraConfig();
}

void CCoreOSG::InitSceneGraph(void)
{
    // ��ʼ����
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
    // �ֲ�������Ŵ��ھ���
    RECT rect;

    // ����һ��viewer
    mViewer = new osgViewer::Viewer();
    
    // �õ���ǰ���ھ���
    ::GetWindowRect(m_hWnd, &rect);

    // ��ʼ��ͼ������ʲô����
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;

    // ��ʼ�����ڱ�����ΪOSG����
    osg::ref_ptr<osg::Referenced> windata = new osgViewer::GraphicsWindowWin32::WindowData(m_hWnd);

    // ����һЩ������
    traits->x = 0;
    traits->y = 0;
    traits->width = rect.right - rect.left;
    traits->height = rect.bottom - rect.top;
	//traits->width = 1000;
	//traits->height = 800;
    traits->windowDecoration = true;
    traits->doubleBuffer = true;
    traits->sharedContext = 0;
	//�������о����Ƿ��¿�����
    traits->setInheritedWindowPixelFormat = true;
    traits->inheritedWindowData = windata;

    // ����ͼ��������
    osg::GraphicsContext* gc = osg::GraphicsContext::createGraphicsContext(traits.get());

	//����������_����ͼ
	double fovy ,aspectRation ,zNear ,zFar;
	mViewer->getCamera()->getProjectionMatrixAsPerspective(fovy,aspectRation,zNear,zFar);
	double newAspectRatio = double(traits->width)/double(traits->height);
	double aspectRatioChange = newAspectRatio/aspectRation;
	if(aspectRatioChange != 1.0)
	{
		mViewer->getCamera()->getProjectionMatrix() *= osg::Matrix::scale(1.0/aspectRatioChange,1.0,1.0);
	}

	mViewer->getCamera()->setViewport(new osg::Viewport(0,0,traits->width,traits->height));

    // ��
    mViewer->getCamera()->setGraphicsContext(gc);

    // ��Ӳ�������VIEWER
    mViewer->setCameraManipulator( g_MyManipulator );

    //��������
    mViewer->setSceneData(mRoot.get());

    // ʵ��VIEWER
    mViewer->realize();
}

void CCoreOSG::PreFrameUpdate()
{
    // ֡ǰ��������������һ��
}

void CCoreOSG::PostFrameUpdate()
{
    // ֡���������������һ��
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
	//�Ƶ�mRoot�����е�����,��Щ���ݽ��ᱻ�Զ��ͷŵ��������ֶ�����
	mRoot ->removeChildren(0, mRoot ->getNumChildren()) ;
   
	//����Ҫ�û���ģ��
	mModel = osgDB::readNodeFile(filename);

	//�Ż�
    osgUtil::Optimizer optimizer;
    optimizer.optimize(mModel.get());
    optimizer.reset();

	//��Ӻ���
    mRoot->addChild(mModel.get());
}
