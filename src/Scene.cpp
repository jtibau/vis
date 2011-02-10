#include "Scene.h"

#include <osgDB/FileUtils>
#include <osgDB/ReadFile>

#include "InputHandler.h"

namespace vis {
	Scene::Scene(){
		mFrameNumber = 0;
		osg::Referenced::setThreadSafeReferenceCounting(true);
	}
	Scene::~Scene(){}
/*
	void Scene::init(InputHandler* input){
		mInput = input;

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
	}
*/
	void Scene::contextInit(){
		const unsigned int unique_context_id = mInput->getCurrentContext();
		// This is the object that will be in charged of rendering everything
		osg::ref_ptr<osgUtil::SceneView> sv(new osgUtil::SceneView);
		sv->setDefaults(osgUtil::SceneView::STANDARD_SETTINGS);

		sv->setFrameStamp(mFrameStamp.get()); // Need to do this before init

		sv->init();
		sv->setClearColor(osg::Vec4( 0, 0, 0, 0 ));
		sv->setDrawBufferValue(GL_NONE); // Needed for stereo to work

		// Set the light
		sv->getLight()->setAmbient(osg::Vec4(0.3f,0.3f,0.3f,1.0f));
		sv->getLight()->setDiffuse(osg::Vec4(0.9f,0.9f,0.9f,1.0f));
		sv->getLight()->setSpecular(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
		sv->getLight()->setPosition(osg::Vec4(0.0f,2.0f,0.0f,1.0f));

		// We tell it its context id
		sv->getState()->setContextID(unique_context_id);

		// Add the tree to the scene viewer and set properties
		mInput->lockMutex();
		sv->setSceneData(mRoot.get());
		mInput->releaseMutex();

		// Set the configured scene viewer object to the context specific pointer.
		(*mSceneView) = sv;
	}

	void Scene::latePreFrame(){
		mFrameStamp->setFrameNumber(++mFrameNumber);
		mFrameStamp->setReferenceTime(mInput->timeStamp());
		mFrameStamp->setSimulationTime(mInput->timeStamp());

		mUpdateVisitor->setTraversalNumber(mFrameNumber);
		mRoot->accept(*mUpdateVisitor);
	}

	void Scene::draw(){
		osg::ref_ptr<osgUtil::SceneView> sv;
		sv = (*mSceneView);	// Get context specific scene viewer

		if(sv.get() != NULL){
			sv->setComputeNearFarMode(osgUtil::CullVisitor::DO_NOT_COMPUTE_NEAR_FAR);

			mInput->getViewport(mViewport);
			sv->setViewport(mViewport[0], mViewport[1], mViewport[2], mViewport[3]);

			mInput->getFrustum(mFrustum);
			sv->setProjectionMatrixAsFrustum(mFrustum[0], mFrustum[1], mFrustum[2], mFrustum[3], mFrustum[4], mFrustum[5]);

			mInput->getViewMatrix(mViewMatrix);
			sv->setViewMatrix(osg::Matrix(mViewMatrix));

			sv->cull();
			sv->draw();
		}
	}

}
