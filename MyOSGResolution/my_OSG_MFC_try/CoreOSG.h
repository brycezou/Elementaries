/* -*-c++-*- OpenSceneGraph - 2008.3.14 by FreeSouth
 *
 * �ÿ����FreeSouth����ԭMDI����޸�,��������ָÿ����BUG���Ե� 
 * bbs.osgChina.org�Ϸ�������,�����¼�����:ieysx@163.com
 *лл��֧��www.osgChina.org
 *����������ĸ���,�޸�,�����ÿ��
*/

//------------------------------//------------------------------
#pragma once

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/api/win32/GraphicsWindowWin32>
#include <osgGA/TrackballManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgDB/DatabasePager>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <string>
//------------------------------//------------------------------
/**OSG�����࣬������ִ��MFC��OSG�Ĵ󲿷ֵĹ�����*/

class MyManipulator;

class CCoreOSG
{
public:
	/**���캯����ȡ��һ�����ھ��*/
    CCoreOSG(HWND hWnd);
    ~CCoreOSG();

	/**��ʼ��OSG,����filenameΪҪ�����ģ���ļ�*/
    //void InitOSG(std::string filename);
	void InitOSG(void);

	/**��ʼ��������*/
    void InitManipulators(void);

	/**��ʼ������*/
    void InitSceneGraph(void);

	/**��ʼ���������*/
    void InitCameraConfig(void);

	/**��������*/
    void SetupWindow(void);

	/**�������*/
    void SetupCamera(void);

	/**ÿ֡����ǰ����*/
    void PreFrameUpdate(void);

	//ÿ֡���ƺ���
    void PostFrameUpdate(void);

	//SetDone
    void Done(bool value) { mDone = value; }

	//getDone
    bool Done(void) { return mDone; }

	//��Ⱦ,ע�������Ǿ�̬��,������Ϊȫ���߳�
    static void Render(void* ptr);

	//ʹ��һ������Ϊfilename��ģ�ʹ��浱ǰ�����е�����ģ��
	void ReplaceSceneData(std::string filename) ;

	//getViewer
    osgViewer::Viewer* getViewer() { return mViewer; }

private:
	//Done
    bool mDone;
	//ģ���ļ�����
    std::string m_ModelName;
	//���ھ��
    HWND m_hWnd;
	//viewer
    osgViewer::Viewer* mViewer;
	//���������
    osg::ref_ptr<osg::Group> mRoot;
	//����ģ���ļ�
    osg::ref_ptr<osg::Node> mModel;
	//trackball������
    osg::ref_ptr<osgGA::TrackballManipulator> trackball;

};//end class CCoreOSG 
//------------------------------//------------------------------