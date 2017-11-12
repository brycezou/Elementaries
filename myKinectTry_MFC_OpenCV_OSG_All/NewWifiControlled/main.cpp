#include "TravelManipulator.h"
#include "mySocket.h"


int main(int argc, char** argv)
{	
	WSADATA wsadata;
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