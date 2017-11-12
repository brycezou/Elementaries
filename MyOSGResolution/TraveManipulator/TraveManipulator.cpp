#include <iostream>
#include "TravelManipulator.h"
using namespace std;

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

TravelManipulator::TravelManipulator():	m_fMoveSpeed(5.0f),
															m_bLeftButtonDown(false),
															m_fPushX(0),
															m_fAngle(5),
															m_bPeng(false),
															m_fPushY(0)
{
	m_vPosition=osg::Vec3(0.0f, -120.0f, 100.0f);
	m_vRotation=osg::Vec3(osg::PI_2, 0.0f, 0.0f);
}
TravelManipulator::~TravelManipulator()
{
}

//�����μӵ�������ȥ
TravelManipulator* TravelManipulator::TravelToScene(osg::ref_ptr<osgViewer::Viewer> viewer)
{
	TravelManipulator* camera=new TravelManipulator;
	viewer->setCameraManipulator(camera);
	camera->m_pHostViewer=viewer;
	return camera;
}

void TravelManipulator::setByMatrix(const osg::Matrixd& matrix)
{

}
void TravelManipulator::setByInverseMatrix(const osg::Matrixd& matrix)
{

}
osg::Matrixd TravelManipulator::getMatrix(void) const
{
	osg::Matrixd mat;
	mat.makeRotate(m_vRotation._v[0], osg::Vec3(1.0f, 0.0f, 0.0f), m_vRotation._v[1], osg::Vec3(0.0f, 1.0f, 0.0f), m_vRotation._v[2], osg::Vec3(0.0f, 0.0f, 1.0f));
	return mat*osg::Matrixd::translate(m_vPosition);
}
osg::Matrixd TravelManipulator::getInverseMatrix(void) const
{
	osg::Matrixd mat;
	mat.makeRotate(m_vRotation._v[0], osg::Vec3(1.0f, 0.0f, 0.0f), m_vRotation._v[1], osg::Vec3(0.0f, 1.0f, 0.0f), m_vRotation._v[2], osg::Vec3(0.0f, 0.0f, 1.0f));
	return osg::Matrixd::inverse(mat * osg::Matrixd::translate(m_vPosition));
}

//�¼�������
bool TravelManipulator::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us)
{
	float mouseX=ea.getX();
	float mouseY=ea.getY();

	switch(ea.getEventType())
	{
	case (osgGA::GUIEventAdapter::KEYDOWN):
		{
			if (ea.getKey()==MF1||ea.getKey()==MF2)	//��ǰƽ��
			{
				ChangePosition(osg::Vec3(m_fMoveSpeed*cosf(osg::PI_2+m_vRotation._v[2]), 0, 0));
				ChangePosition(osg::Vec3(0, m_fMoveSpeed*sinf(osg::PI_2+m_vRotation._v[2]), 0));
				return true;
			}
			if (ea.getKey()==MB1||ea.getKey()==MB2)	//���ƽ��
			{
				ChangePosition(osg::Vec3(-m_fMoveSpeed*cosf(osg::PI_2+m_vRotation._v[2]), 0, 0));
				ChangePosition(osg::Vec3(0, -m_fMoveSpeed*sinf(osg::PI_2+m_vRotation._v[2]), 0));
				return true;
			}
			if (ea.getKey()==ML1||ea.getKey()==ML2)	//����ƽ��
			{
				ChangePosition(osg::Vec3(-m_fMoveSpeed*sinf(osg::PI_2+m_vRotation._v[2]), 0, 0));
				ChangePosition(osg::Vec3(0, m_fMoveSpeed*cosf(osg::PI_2+m_vRotation._v[2]), 0));
				return true;
			}
			if (ea.getKey()==MR1||ea.getKey()==MR2)	//����ƽ��
			{
				ChangePosition(osg::Vec3(m_fMoveSpeed*sinf(osg::PI_2+m_vRotation._v[2]), 0, 0));
				ChangePosition(osg::Vec3(0, -m_fMoveSpeed*cosf(osg::PI_2+m_vRotation._v[2]), 0));
				return true;
			}

			if (ea.getKey()==RR)	//������ת
			{
				m_vRotation._v[2]-=osg::DegreesToRadians(m_fAngle);
			}
			if (ea.getKey()==RL)	//������ת
			{
				m_vRotation._v[2]+=osg::DegreesToRadians(m_fAngle);
			}
			if (ea.getKey()==RU)	//������ת
			{
				m_vRotation._v[0]+=osg::DegreesToRadians(0.5*m_fAngle);
				if (m_vRotation._v[0]>=osg::PI*7/12)
				{
					m_vRotation._v[0]=osg::PI*7/12;
				}
				if (m_vRotation._v[0]<=osg::PI*5/12)	//between 1/3 and 1/2 pi
				{
					m_vRotation._v[0]=osg::PI*5/12;
				}
			}
			if (ea.getKey()==RD)	//������ת
			{
				m_vRotation._v[0]-=osg::DegreesToRadians(0.5*m_fAngle);
				if (m_vRotation._v[0]>=osg::PI*7/12)
				{
					m_vRotation._v[0]=osg::PI*7/12;
				}
				if (m_vRotation._v[0]<=osg::PI*5/12)	//between 1/3 and 1/2 pi
				{
					m_vRotation._v[0]=osg::PI*5/12;
				}
			}

			return false;
		}

		//��갴��
	case (osgGA::GUIEventAdapter::PUSH):
		if (ea.getButton()==1)
		{
			m_fPushX=mouseX;
			m_fPushY=mouseY;
			m_bLeftButtonDown=true;
		}
		return false;

		//�϶�
	case (osgGA::GUIEventAdapter::DRAG):
		if (m_bLeftButtonDown)
		{
			m_vRotation._v[2]-=osg::DegreesToRadians(0.05*m_fAngle*(mouseX-m_fPushX));
			m_vRotation._v[0]+=osg::DegreesToRadians(0.05*m_fAngle*(mouseY-m_fPushY));
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

		//����ͷ�
	case (osgGA::GUIEventAdapter::RELEASE):
		if (ea.getButton()==1)
		{
			m_bLeftButtonDown=false;
		}
		return false;

	default:
		return false;
	}
}

void TravelManipulator::ChangePosition(osg::Vec3& delta)
{
	//��ײ���
	if(m_bPeng)
	{
		//�õ��µ�λ��
		osg::Vec3 newPos1=m_vPosition+delta;

		osgUtil::IntersectVisitor ivXY;
		//�����µ�λ�õõ������߶μ��
		osg::ref_ptr<osg::LineSegment>lineXY=new osg::LineSegment(newPos1, m_vPosition);
		osg::ref_ptr<osg::LineSegment>lineZ=new osg::LineSegment(newPos1+osg::Vec3(0.0f, 0.0f, 10.0f), newPos1-osg::Vec3(0.0f, 0.0f, 10.0f));

		ivXY.addLineSegment(lineZ.get());
		ivXY.addLineSegment(lineXY.get());

		//�ṹ�������
		m_pHostViewer->getSceneData()->accept(ivXY);
		//���û����ײ���
		if (!ivXY.hits())
		{
			m_vPosition+=delta;
		}
	}
	else
	{
		m_vPosition+=delta;
	}
}

//�����ٶ�
void TravelManipulator::setSpeed(float sp)
{
	m_fMoveSpeed=sp;
}
float TravelManipulator::getSpeed()
{
	return m_fMoveSpeed;
}

//������ʼ��λ��
void TravelManipulator::SetPosition(osg::Vec3 &position)
{
	m_vPosition=position;
}
osg::Vec3 TravelManipulator::GetPosition()
{
	return m_vPosition;
}
