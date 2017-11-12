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


//��ȡ����ͼ
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
		//��������ͼ��6�������ͼ
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_X, imagePosX.get());
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_X, imageNegX.get());
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_Y, imagePosY.get());
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Y, imageNegY.get());
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_Z, imagePosZ.get());
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Z, imageNegZ.get());

		//����������ģʽ
		cubemap->setWrap(osg::Texture::WRAP_S,osg::Texture::CLAMP_TO_EDGE);
		cubemap->setWrap(osg::Texture::WRAP_T,osg::Texture::CLAMP_TO_EDGE);
		cubemap->setWrap(osg::Texture::WRAP_R,osg::Texture::CLAMP_TO_EDGE);

		//�����˲������κ�mipmap
		cubemap->setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR_MIPMAP_LINEAR);
		cubemap->setFilter(osg::Texture::MAG_FILTER,osg::Texture::LINEAR);
	}

	return cubemap.get();
}

//����������ͼ����
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
			//�õ�ģ����ͼ����������ת�Ƕ�
			const osg::Matrix& MV=*(cv->getModelViewMatrix());
			const osg::Matrix R=osg::Matrix::rotate(osg::DegreesToRadians(120.0f), 0.0f, 0.0f, 1.0f)*osg::Matrix::rotate(osg::DegreesToRadians(90.0f), 1.0f, 0.0f, 0.0f);
			osg::Quat q=MV.getRotate();
			const osg::Matrix C=osg::Matrix::rotate(q.inverse());

			_texMat.setMatrix(C*R);			//�����������
		}
		traverse(node, nv);
	}

	osg::TexMat& _texMat;		//�������
};

//һ���任�࣬ʹ��պ����ӵ���ת
class MoveEarthySkyWithEyePointTransform :public osg::Transform
{
public:
	//�ֲ����������������
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

	//����������Ϊ�ֲ�����
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

//������պ�
osg::ref_ptr<osg::Node> createSkyBox()
{
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();

	//��������ӳ�䷽ʽ���ƶ�Ϊ�����ʽ���������е���ɫ����ԭ������ɫ
	osg::ref_ptr<osg::TexEnv> te = new osg::TexEnv;
	te->setMode(osg::TexEnv::REPLACE);
	stateset->setTextureAttributeAndModes(1, te.get(),osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	//�Զ������������꣬���䷽ʽ��REFELCTION_MAP��
	/*
	NORMAL_MAP ��׼ģʽ������������
	REFELCTION_MAP ����ģʽ����������
	SPHERE_MAP ����ģ�͡���������
	*/
	osg::ref_ptr<osg::TexGen> tg = new osg::TexGen;
	tg->setMode(osg::TexGen::NORMAL_MAP);
	stateset->setTextureAttributeAndModes(0, tg.get(),osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	//�����������
	osg::ref_ptr<osg::TexMat> tm =new osg::TexMat;
	stateset->setTextureAttribute(0, tm.get());

	//��������ͼ����
	osg::ref_ptr<osg::TextureCubeMap> skymap = readCubeMap();
	stateset->setTextureAttributeAndModes(0,skymap.get(),osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	stateset->setMode(GL_CULL_FACE,osg::StateAttribute::OFF);

	//���������ΪԶƽ��
	osg::ref_ptr<osg::Depth> depth = new osg::Depth;
	depth->setFunction(osg::Depth::ALWAYS);
	depth->setRange(1.0, 1.0);
	stateset->setAttributeAndModes(depth,osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	//����Ⱦ˳������Ϊ-1������Ⱦ
	stateset->setRenderBinDetails(-1, "RenderBin");

	osg::ref_ptr<osg::Drawable> drawable = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f, 0.0f, 0.0f), 1));

	//����������뵽Ҷ�ڵ���
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->setCullingActive(false);
	geode->setStateSet(stateset.get());
	geode->addDrawable(drawable.get());

	//���ñ任
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