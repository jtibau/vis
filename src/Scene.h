#pragma once

#include <vrj/vrjConfig.h>
#include <vrj/Draw/OpenGL/ContextData.h>

#include <osg/Group>
#include <osg/MatrixTransform>
#include <osgUtil/SceneView>
#include <osgUtil/UpdateVisitor>

#include <osgDB/FileUtils>
#include <osgDB/ReadFile>

namespace vis {

	class InputHandler;

	class Scene {

	public:
		Scene();
		~Scene();

		void init(InputHandler* input){
			mUpdateVisitor = new osgUtil::UpdateVisitor();
			mFrameStamp    = new osg::FrameStamp();
			mUpdateVisitor->setFrameStamp(mFrameStamp.get());

			// root -- navigation -- modelScale -- modelTranslationRotation -- model
			mRoot = new osg::Group();
			osg::MatrixTransform* navMT = new osg::MatrixTransform();
			{
				osg::Group* modelNode = new osg::Group();
				modelNode->setName("Model");
				osg::MatrixTransform* scaleMT = new osg::MatrixTransform();
				osg::MatrixTransform* translateRotateMT = new osg::MatrixTransform();

				translateRotateMT->addChild(osgDB::readNodeFile("cow.osg"));
				scaleMT->addChild(translateRotateMT);
				modelNode->addChild(scaleMT);
				navMT->addChild(modelNode);
				mRoot->addChild(navMT);
			}
			mInput = input;
		}

		void contextInit();
		void latePreFrame();
		void draw();

	protected:
		InputHandler* mInput;

		vrj::opengl::ContextData< osg::ref_ptr< osgUtil::SceneView > >	mSceneView;
		osg::ref_ptr< osg::NodeVisitor >								mUpdateVisitor;
		osg::ref_ptr< osg::FrameStamp >									mFrameStamp;
		int																mFrameNumber;

		osg::ref_ptr< osg::Group >										mRoot;
		osg::ref_ptr< osg::MatrixTransform >							mSelectedObjectTransformation;
		osg::ref_ptr< osg::MatrixTransform >							mSelectedObjectScaling;

		int mViewport[4];
		float mFrustum[6];
		float mViewMatrix[16];
	};
}
