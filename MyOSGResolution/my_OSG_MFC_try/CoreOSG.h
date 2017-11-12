/* -*-c++-*- OpenSceneGraph - 2008.3.14 by FreeSouth
 *
 * 该框架是FreeSouth基于原MDI框架修改,如果您发现该框架有BUG可以到 
 * bbs.osgChina.org上发帖声明,或是致件作者:ieysx@163.com
 *谢谢您支持www.osgChina.org
 *您可以随意的复制,修改,传播该框架
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
/**OSG核心类，在这里执行MFC对OSG的大部分的管理功能*/

class MyManipulator;

class CCoreOSG
{
public:
	/**构造函数获取上一级窗口句柄*/
    CCoreOSG(HWND hWnd);
    ~CCoreOSG();

	/**初始化OSG,参数filename为要加入的模型文件*/
    //void InitOSG(std::string filename);
	void InitOSG(void);

	/**初始化操作器*/
    void InitManipulators(void);

	/**初始化场景*/
    void InitSceneGraph(void);

	/**初始化相机配置*/
    void InitCameraConfig(void);

	/**建立窗口*/
    void SetupWindow(void);

	/**建立相机*/
    void SetupCamera(void);

	/**每帧绘制前处理*/
    void PreFrameUpdate(void);

	//每帧绘制后处理
    void PostFrameUpdate(void);

	//SetDone
    void Done(bool value) { mDone = value; }

	//getDone
    bool Done(void) { return mDone; }

	//渲染,注意这里是静态的,可启用为全局线程
    static void Render(void* ptr);

	//使用一个名称为filename的模型代替当前场景中的所有模型
	void ReplaceSceneData(std::string filename) ;

	//getViewer
    osgViewer::Viewer* getViewer() { return mViewer; }

private:
	//Done
    bool mDone;
	//模型文件名称
    std::string m_ModelName;
	//窗口句柄
    HWND m_hWnd;
	//viewer
    osgViewer::Viewer* mViewer;
	//场景根结点
    osg::ref_ptr<osg::Group> mRoot;
	//场景模型文件
    osg::ref_ptr<osg::Node> mModel;
	//trackball操作器
    osg::ref_ptr<osgGA::TrackballManipulator> trackball;

};//end class CCoreOSG 
//------------------------------//------------------------------