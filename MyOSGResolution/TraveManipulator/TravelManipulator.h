#pragma once
#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Point>
#include <osg/Geometry>
#include <osgUtil/IntersectVisitor>
#include <osgGA/CameraManipulator>
#include <vector>

class TravelManipulator:public osgGA::CameraManipulator
{
public:
	TravelManipulator();
	~TravelManipulator();
	static TravelManipulator* TravelToScene(osg::ref_ptr<osgViewer::Viewer> viewer);//�����������뵽������
private:
	osg::ref_ptr<osgViewer::Viewer> m_pHostViewer;
	float m_fMoveSpeed;
	osg::Vec3 m_vPosition;
	osg::Vec3 m_vRotation;
public:
	bool m_bLeftButtonDown;
	float m_fPushX;
	float m_fPushY;
	virtual void setByMatrix(const osg::Matrixd& matrix);//���þ���
	virtual void setByInverseMatrix(const osg::Matrixd& matrix);//���������
	virtual osg::Matrixd getMatrix(void) const;//�õ�����
	virtual osg::Matrixd getInverseMatrix(void) const;//�õ������
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);//�¼�������
	float m_fAngle;//��Ļ�Ƕ�
	void ChangePosition(osg::Vec3& delta);//λ�ñ任����
	bool m_bPeng;//��ײ����Ƿ���
	float getSpeed();
	void setSpeed(float sp);//�����ٶ�
	osg::Vec3 GetPosition();
	void SetPosition(osg::Vec3 &position);//������ʼλ��
};

