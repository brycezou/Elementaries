#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgUtil/Optimizer>
#include <osgViewer/ViewerEventHandlers>
#include "TravelManipulator.h"

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();//创建场景浏览器
	TravelManipulator::TravelToScene(viewer.get());//把漫游器加入到场景中
	osg::ref_ptr<osg::Group> root = new osg::Group();
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("lz.osg");
	root->addChild(node.get());

	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());
	viewer->addEventHandler(new osgViewer::WindowSizeHandler);
	viewer->realize();
	viewer->run();

	return 0;
}
