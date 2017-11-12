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
	static TravelManipulator* TravelToScene(osg::ref_ptr<osgViewer::Viewer> viewer);//把漫游器加入到场景中
private:
	osg::ref_ptr<osgViewer::Viewer> m_pHostViewer;
	float m_fMoveSpeed;
	osg::Vec3 m_vPosition;
	osg::Vec3 m_vRotation;
public:
	bool m_bLeftButtonDown;
	float m_fPushX;
	float m_fPushY;
	virtual void setByMatrix(const osg::Matrixd& matrix);//设置矩阵
	virtual void setByInverseMatrix(const osg::Matrixd& matrix);//设置逆矩阵
	virtual osg::Matrixd getMatrix(void) const;//得到矩阵
	virtual osg::Matrixd getInverseMatrix(void) const;//得到逆矩阵
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);//事件处理函数
	float m_fAngle;//屏幕角度
	void ChangePosition(osg::Vec3& delta);//位置变换函数
	bool m_bPeng;//碰撞检测是否开启
	float getSpeed();
	void setSpeed(float sp);//设置速度
	osg::Vec3 GetPosition();
	void SetPosition(osg::Vec3 &position);//设置起始位置
};

