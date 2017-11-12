#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Quat>
#include <osg/Matrix>
#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Transform>
#include <osg/Material>
#include <osg/NodeCallback>
#include <osg/Depth>
#include <osg/CullFace>
#include <osg/TexMat>
#include <osg/TexGen>
#include <osg/TexEnv>
#include <osg/TextureCubeMap>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>


//读取立方图
osg::ref_ptr<osg::TextureCubeMap> readCubeMap()
{
	osg::ref_ptr<osg::TextureCubeMap> cubemap = new osg::TextureCubeMap;

	//osg::ref_ptr<osg::Image> imagePosX = osgDB::readImageFile("Cubemap_axis/posx.bmp");
	//osg::ref_ptr<osg::Image> imageNegX = osgDB::readImageFile("Cubemap_axis/negx.bmp");
	//osg::ref_ptr<osg::Image> imagePosY = osgDB::readImageFile("Cubemap_axis/posy.bmp");
	//osg::ref_ptr<osg::Image> imageNegY = osgDB::readImageFile("Cubemap_axis/negy.bmp");
	//osg::ref_ptr<osg::Image> imagePosZ = osgDB::readImageFile("Cubemap_axis/posz.bmp");
	//osg::ref_ptr<osg::Image> imageNegZ = osgDB::readImageFile("Cubemap_axis/negz.bmp");

	osg::ref_ptr<osg::Image> imagePosX = osgDB::readImageFile("Cubemap_axis/1.bmp");
	osg::ref_ptr<osg::Image> imageNegX = osgDB::readImageFile("Cubemap_axis/2.bmp");
	osg::ref_ptr<osg::Image> imagePosY = osgDB::readImageFile("Cubemap_axis/3.bmp");
	osg::ref_ptr<osg::Image> imageNegY = osgDB::readImageFile("Cubemap_axis/4.bmp");
	osg::ref_ptr<osg::Image> imagePosZ = osgDB::readImageFile("Cubemap_axis/5.bmp");
	osg::ref_ptr<osg::Image> imageNegZ = osgDB::readImageFile("Cubemap_axis/6.bmp");

	if(imagePosX.get() && imageNegX.get() && imagePosY.get() && imageNegY.get() && imagePosZ.get() && imageNegZ.get())
	{
		//设置立方图的6个面的贴图
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_X, imagePosX.get());
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_X, imageNegX.get());
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_Y, imagePosY.get());
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Y, imageNegY.get());
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_Z, imagePosZ.get());
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Z, imageNegZ.get());

		//设置纹理环绕模式
		cubemap->setWrap(osg::Texture::WRAP_S,osg::Texture::CLAMP_TO_EDGE);
		cubemap->setWrap(osg::Texture::WRAP_T,osg::Texture::CLAMP_TO_EDGE);
		cubemap->setWrap(osg::Texture::WRAP_R,osg::Texture::CLAMP_TO_EDGE);

		//设置滤波：线形和mipmap
		cubemap->setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR_MIPMAP_LINEAR);
		cubemap->setFilter(osg::Texture::MAG_FILTER,osg::Texture::LINEAR);
	}

	return cubemap.get();
}

//更新立方体图纹理
struct TexMatCallBack : public osg::NodeCallback
{
public:
	TexMatCallBack(osg::TexMat& tm): _texMat(tm)
	{

	}

	virtual void operator()(osg::Node*node,osg::NodeVisitor* nv)
	{
		osgUtil::CullVisitor* cv=dynamic_cast<osgUtil::CullVisitor*>(nv);
		if(cv)
		{
			//得到模型视图矩阵并设置旋转角度
			const osg::Matrix& MV=*(cv->getModelViewMatrix());
			const osg::Matrix R=osg::Matrix::rotate(osg::DegreesToRadians(120.0f), 0.0f, 0.0f, 1.0f)*osg::Matrix::rotate(osg::DegreesToRadians(90.0f), 1.0f, 0.0f, 0.0f);
			osg::Quat q=MV.getRotate();
			const osg::Matrix C=osg::Matrix::rotate(q.inverse());

			_texMat.setMatrix(C*R);			//设置纹理矩阵
		}
		traverse(node, nv);
	}

	osg::TexMat& _texMat;		//纹理矩阵
};

//一个变换类，使天空盒绕视点旋转
class MoveEarthySkyWithEyePointTransform :public osg::Transform
{
public:
	//局部矩阵计算成世界矩阵
	virtual bool computeLocalToWorldMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv)const
	{
		osgUtil::CullVisitor* cv=dynamic_cast<osgUtil::CullVisitor*>(nv);
		if(cv)
		{
			osg::Vec3 eyePointLocal=cv->getEyeLocal();
			matrix.preMult(osg::Matrix::translate(eyePointLocal));
		}
		return true;
	}

	//世界矩阵计算为局部矩阵
	virtual bool computeWorldToLocalMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv)const
	{
		osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
		if(cv)
		{
			osg::Vec3 eyePointLocal = cv->getEyeLocal();
			matrix.postMult(osg::Matrix::translate(-eyePointLocal));
		}
		return true;
	}
};

//创建天空盒
osg::ref_ptr<osg::Node> createSkyBox()
{
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();

	//设置纹理映射方式，制定为替代方式，即纹理中的颜色代替原来的颜色
	osg::ref_ptr<osg::TexEnv> te = new osg::TexEnv;
	te->setMode(osg::TexEnv::REPLACE);
	stateset->setTextureAttributeAndModes(1, te.get(),osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	//自动生成纹理坐标，反射方式（REFELCTION_MAP）
	/*
	NORMAL_MAP 标准模式—立方体纹理
	REFELCTION_MAP 反射模式—球体纹理
	SPHERE_MAP 球体模型—球体纹理
	*/
	osg::ref_ptr<osg::TexGen> tg = new osg::TexGen;
	tg->setMode(osg::TexGen::NORMAL_MAP);
	stateset->setTextureAttributeAndModes(0, tg.get(),osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	//设置纹理矩阵
	osg::ref_ptr<osg::TexMat> tm =new osg::TexMat;
	stateset->setTextureAttribute(0, tm.get());

	//设置立方图纹理
	osg::ref_ptr<osg::TextureCubeMap> skymap = readCubeMap();
	stateset->setTextureAttributeAndModes(0,skymap.get(),osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	stateset->setMode(GL_CULL_FACE,osg::StateAttribute::OFF);

	//将深度设置为远平面
	osg::ref_ptr<osg::Depth> depth = new osg::Depth;
	depth->setFunction(osg::Depth::ALWAYS);
	depth->setRange(1.0, 1.0);
	stateset->setAttributeAndModes(depth,osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	//讲渲染顺序设置为-1，先渲染
	stateset->setRenderBinDetails(-1, "RenderBin");

	osg::ref_ptr<osg::Drawable> drawable = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f, 0.0f, 0.0f), 1));

	//把立方体加入到叶节点中
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->setCullingActive(false);
	geode->setStateSet(stateset.get());
	geode->addDrawable(drawable.get());

	//设置变换
	osg::ref_ptr<osg::Transform> transform = new MoveEarthySkyWithEyePointTransform();
	transform->setCullingActive(false);
	transform->addChild(geode.get());

	osg::ref_ptr<osg::ClearNode>clearNode = new osg::ClearNode;
	clearNode->setCullCallback(new TexMatCallBack(*tm));
	clearNode->addChild(transform.get());

	return clearNode.get();
}

int main()
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	osg::ref_ptr<osg::Group> root = new osg::Group();
	osg::ref_ptr<osg::Node> node = createSkyBox();
	root->addChild(node.get());
	//osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
	//pat->setPosition(osg::Vec3(0.0f, 0.0f, 0.0f));
	//pat->setScale(osg::Vec3(0.5f, 0.5f, 0.5f));
	//pat->addChild(node.get());
	//root->addChild(pat.get());

	//osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
	//osg::Matrix m;
	//m.makeTranslate(osg::Vec3(100.0f, 0.0f, 0.0f));
	//mt->setMatrix(m);
	//mt->addChild(node.get());
	//root->addChild(mt.get());

	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());
	viewer->addEventHandler(new osgViewer::WindowSizeHandler);
	viewer->realize();
	viewer->run();

	return 0;
}