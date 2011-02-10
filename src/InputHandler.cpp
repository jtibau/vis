#include "InputHandler.h"

#include <gmtl/Generate.h>
#include <gmtl/VecOps.h>
#include <gmtl/MatrixOps.h>

#include <iostream>

namespace vis {
	InputHandler::InputHandler() {}
	InputHandler::~InputHandler() {};

	void InputHandler::init(){
		mHead.init("VJHead");
		mHand.init("VJHand");

		for(int i=0; i<MAX_BUTTONS; i++){
			std::stringstream buttonName;
			buttonName << "VJButton" << i;
			mButton[i].init(buttonName.str());
			mButtonState[i] = false;
			mButtonToggle[i] = false;
			mFirstButtonPress[i] = true;
		}

		mTimestamp[0] = 0;
		mTimestamp[1] = 0;

		mNavigation = false;
		mIntersectionTest = false;
		mObjectIntersected = false;
		mManipulation = false;
		mScale = false;

		//mNavigationMatrix = gmtl::Matrix44f::IDENTITY;
		//mSelectedObjectTransformation = gmtl::Matrix44f::IDENTITY;
		//mSelectedObjectScale = gmtl::Matrix44f::IDENTITY;

		//mHeadMatrix[CURRENT] = gmtl::Matrix44f::IDENTITY;
		mHeadPosition[CURRENT] = gmtl::Vec3f(0,0,0);
		mHeadDirection[CURRENT] = gmtl::Vec3f(0,0,-1);

		//mHandMatrix[CURRENT] = gmtl::Matrix44f::IDENTITY;
		mHandPosition[CURRENT] = gmtl::Vec3f(0,0,0);
		mHandDirection[CURRENT] = gmtl::Vec3f(0,0,-1);
	}

	void InputHandler::update(){
		mTimestamp[PREVIOUS] = mTimestamp[CURRENT];
		mTimestamp[CURRENT] = mHead->getTimeStamp().secd();
		mTimeDelta = (float)(mTimestamp[CURRENT] - mTimestamp[PREVIOUS]);

		// store previous head and wand data
		mHeadMatrix[PREVIOUS] = mHeadMatrix[CURRENT];
		mHeadPosition[PREVIOUS] = mHeadPosition[CURRENT];
		mHeadDirection[PREVIOUS] = mHeadDirection[CURRENT];

		mHandMatrix[PREVIOUS] = mHandMatrix[CURRENT];
		mHandPosition[PREVIOUS] = mHandPosition[CURRENT];
		mHandDirection[PREVIOUS] = mHandDirection[CURRENT];


		// update head/hand data
		gmtl::Vec3f Zdir = gmtl::Vec3f(0,0,-1);	// forward vector

		mHeadMatrix[CURRENT] = mHead->getData(gadget::PositionUnitConversion::ConvertToMeters);
		gmtl::xform(mHeadDirection[CURRENT], mHeadMatrix[CURRENT], Zdir);
		mHeadPosition[CURRENT] = gmtl::makeTrans<gmtl::Point3f>(mHeadMatrix[CURRENT]);

		mHandMatrix[CURRENT] = mHand->getData(gadget::PositionUnitConversion::ConvertToMeters);
		gmtl::xform(mHandDirection[CURRENT], mHandMatrix[CURRENT], Zdir);
		mHandPosition[CURRENT] = gmtl::makeTrans<gmtl::Point3f>(mHandMatrix[CURRENT]);

		// update button data only if the digital interface reports a toggle
		for(int i=0; i<MAX_BUTTONS; i++){
			if(!mFirstButtonPress[i]){
				mButtonToggle[i] = false;
				if( mButton[i]->getData() == gadget::Digital::TOGGLE_ON )
					mButtonToggle[i] = mButtonState[i] = true;
				if( mButton[i]->getData() == gadget::Digital::TOGGLE_OFF )
					mButtonToggle[i] = !(mButtonState[i] = false);	// ;)
			}
			else if( mButton[i]->getData() == gadget::Digital::TOGGLE_OFF )
				mFirstButtonPress[i] = false;
		}
	}

	// System

	unsigned int InputHandler::getCurrentContext(){
		return vrj::opengl::DrawManager::instance()->getCurrentContext();
	}

	void InputHandler::getViewport(int* viewport){
		vrj::opengl::DrawManager* gl_manager = vrj::opengl::DrawManager::instance();

		vprASSERT(gl_manager != NULL)

		vrj::opengl::UserData* user_data = gl_manager->currentUserData();

		float vp_ox, vp_oy, vp_sx, vp_sy;   // The float vrj sizes of the view ports
		int w_ox, w_oy, w_width, w_height;  // Origin and size of the window
		user_data->getViewport()->getOriginAndSize(vp_ox, vp_oy, vp_sx, vp_sy);
		user_data->getGlWindow()->getOriginSize(w_ox, w_oy, w_width, w_height);

		// compute unsigned versions of the viewport info (for passing to glViewport)
		viewport[0] = static_cast<int>(vp_ox * static_cast<float>(w_width));
		viewport[1] = static_cast<int>(vp_oy * static_cast<float>(w_height));
		viewport[2] = static_cast<int>(vp_sx * static_cast<float>(w_width));
		viewport[3] = static_cast<int>(vp_sy * static_cast<float>(w_height));
	}

	void InputHandler::getViewMatrix(float* viewMatrix){
		vrj::opengl::DrawManager* gl_manager = vrj::opengl::DrawManager::instance();
		vprASSERT(gl_manager != NULL)
		vrj::opengl::UserData* user_data = gl_manager->currentUserData();
		vrj::ProjectionPtr project = user_data->getProjection();
		for(int i=0; i<16; i++) viewMatrix[i] = project->getViewMatrix().mData[i];
	}

	void InputHandler::getFrustum(float* frustum){
		vrj::opengl::DrawManager* gl_manager = vrj::opengl::DrawManager::instance();
		vprASSERT(gl_manager != NULL)
		vrj::opengl::UserData* user_data = gl_manager->currentUserData();

		vrj::ProjectionPtr project = user_data->getProjection();
		vrj::Frustum frust = project->getFrustum();

		frustum[0] = frust[vrj::Frustum::VJ_LEFT];
		frustum[1] = frust[vrj::Frustum::VJ_RIGHT];
		frustum[2] = frust[vrj::Frustum::VJ_BOTTOM];
		frustum[3] = frust[vrj::Frustum::VJ_TOP];
		frustum[4] = frust[vrj::Frustum::VJ_NEAR];
		frustum[5] = frust[vrj::Frustum::VJ_FAR];
	}

	void InputHandler::lockMutex(){	mGuard = new vpr::Guard<vpr::Mutex>(mLock);	}
	void InputHandler::releaseMutex(){ delete mGuard; }

	double InputHandler::timeStamp(Time t){
		if(t == CURRENT) return mTimestamp[CURRENT];
		else return mTimestamp[PREVIOUS];
	}

	float InputHandler::timeDelta(){ return mTimeDelta; }

	bool InputHandler::buttonState(unsigned int number){
		if(number < MAX_BUTTONS) return mButtonState[number];
		else return false;
	}

	bool InputHandler::buttonToggle(unsigned int number){
		if(number < MAX_BUTTONS) return mButtonToggle[number];
		else return false;
	}

	gmtl::Matrix44f InputHandler::headMatrix(Time t){
		if(t == CURRENT) return mHeadMatrix[CURRENT];
		else return mHeadMatrix[PREVIOUS];
	}
	gmtl::Vec3f InputHandler::headPosition(Time t){
		if(t == CURRENT) return mHeadPosition[CURRENT];
		else return mHeadPosition[PREVIOUS];
	}
	gmtl::Vec3f InputHandler::headDirection(Time t){
		if(t == CURRENT) return mHeadDirection[CURRENT];
		else return mHeadDirection[PREVIOUS];
	}

	gmtl::Matrix44f InputHandler::handMatrix(Time t){
		if(t == CURRENT) return mHandMatrix[CURRENT];
		else return mHandMatrix[PREVIOUS];
	}
	gmtl::Vec3f InputHandler::handPosition(Time t){
		if(t == CURRENT) return mHandPosition[CURRENT];
		else return mHandPosition[PREVIOUS];
	}
	gmtl::Vec3f InputHandler::handDirection(Time t){
		if(t == CURRENT) return mHandDirection[CURRENT];
		else return mHandDirection[PREVIOUS];
	}

	gmtl::Vec3f InputHandler::rayStart(){ return mRayStart; }
	void InputHandler::rayStart(gmtl::Vec3f point){ mRayStart = point; }
	gmtl::Vec3f InputHandler::rayEnd(){ return mRayEnd; }
	void InputHandler::rayEnd(gmtl::Vec3f point){ mRayEnd = point; }

	// Interaction
	bool InputHandler::applyNavigation(){ return mNavigation; }
	void InputHandler::applyNavigation(bool flag){ mNavigation = flag; }
	gmtl::Matrix44f InputHandler::navigationMatrix(){ return mNavigationMatrix; }
	void InputHandler::navigationMatrix(gmtl::Matrix44f matrix){ mNavigationMatrix = matrix; }

	bool InputHandler::applyIntersectionTest(){ return mIntersectionTest; }
	void InputHandler::applyIntersectionTest(bool flag){ mIntersectionTest = flag; }

	bool InputHandler::objectIntersected(){ return mObjectIntersected; }
	void InputHandler::objectIntersected(bool flag){ mObjectIntersected = flag; }

	bool InputHandler::applyManipulation(){ return mManipulation; }
	void InputHandler::applyManipulation(bool flag){ mManipulation = flag; }
	gmtl::Matrix44f InputHandler::selectedObjectTransformationMatrix(){ return mSelectedObjectTransformation; }
	void InputHandler::selectedObjectTransformationMatrix(gmtl::Matrix44f matrix){ mSelectedObjectTransformation = matrix; }

	bool InputHandler::applyScaleManipulation(){ return mScale; }
	void InputHandler::applyScaleManipulation(bool flag){ mScale = flag; }
	gmtl::Matrix44f InputHandler::selectedObjectScaleMatrix(){ return mSelectedObjectScale; }
	void InputHandler::selectedObjectScaleMatrix(gmtl::Matrix44f matrix){ mSelectedObjectScale = matrix; }
}
