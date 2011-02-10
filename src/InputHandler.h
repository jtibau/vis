#pragma once

#include <vrj/vrjConfig.h>

#include <gmtl/Vec.h>
#include <gmtl/Matrix.h>

#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DigitalInterface.h>

#include <vrj/Draw/OpenGL/Window.h>
#include <vrj/Draw/OpenGL/DrawManager.h>
#include <vrj/Display/CameraProjection.h>
#include <vrj/Kernel/User.h>

#include <string>

#define MAX_BUTTONS 13

namespace vis {

	enum Time {
		CURRENT, PREVIOUS
	};

	class InputHandler {

	public:
		InputHandler();
		~InputHandler();

		void init();
		void update();

		// System
		unsigned int getCurrentContext();
		void getViewport(int* viewport);
		void getViewMatrix(float* viewMatrix);
		void getFrustum(float* frustum);

		void lockMutex();
		void releaseMutex();

		double timeStamp(Time t = CURRENT);
		float timeDelta();

		// Input Data

		bool buttonState(unsigned int number);
		bool buttonToggle(unsigned int number);

		gmtl::Matrix44f headMatrix(Time t = CURRENT);
		gmtl::Vec3f headPosition(Time t = CURRENT);
		gmtl::Vec3f headDirection(Time t = CURRENT);

		gmtl::Matrix44f handMatrix(Time t = CURRENT);
		gmtl::Vec3f handPosition(Time t = CURRENT);
		gmtl::Vec3f handDirection(Time t = CURRENT);

		gmtl::Vec3f rayStart();
		void rayStart(gmtl::Vec3f point);
		gmtl::Vec3f rayEnd();
		void rayEnd(gmtl::Vec3f point);

		// Interaction
		bool applyNavigation();
		void applyNavigation(bool flag);
		gmtl::Matrix44f navigationMatrix();
		void navigationMatrix(gmtl::Matrix44f matrix);

		bool applyIntersectionTest();
		void applyIntersectionTest(bool flag);

		bool objectIntersected();
		void objectIntersected(bool flag);

		bool applyManipulation();
		void applyManipulation(bool flag);
		gmtl::Matrix44f selectedObjectTransformationMatrix();
		void selectedObjectTransformationMatrix(gmtl::Matrix44f matrix);

		bool applyScaleManipulation();
		void applyScaleManipulation(bool flag);
		gmtl::Matrix44f selectedObjectScaleMatrix();
		void selectedObjectScaleMatrix(gmtl::Matrix44f matrix);

	protected:
		gadget::PositionInterface	mHead;							/**< The VRJuggler pointer to the Head */
		gmtl::Matrix44f				mHeadMatrix[2];					/**< Unprocessed head matrix */
		gmtl::Vec3f					mHeadPosition[2];				/**< Head position */
		gmtl::Vec3f					mHeadDirection[2];				/**< Head direction */

		gadget::PositionInterface	mHand;							/**< The VRJuggler pointer to the Hand */
		gmtl::Matrix44f				mHandMatrix[2];					/**< Unprocessed Hand matrix */
		gmtl::Vec3f					mHandPosition[2];				/**< Hand position */
		gmtl::Vec3f					mHandDirection[2];				/**< Hand direction */

		gadget::DigitalInterface	mButton[MAX_BUTTONS];			/**< The VR Juggler Buttons */
		bool						mFirstButtonPress[MAX_BUTTONS];	/**< A hack, to handle a current bug with the VRPN driver in vrj */
		bool						mButtonState[MAX_BUTTONS];		/**< State of the buttons */
		bool						mButtonToggle[MAX_BUTTONS];		/**< Buttons' state change flag */

		double						mTimestamp[2];					/**< Timestamps */
		float						mTimeDelta;						/**< Time between timestamps */

		bool						mNavigation;					/**< Apply Navigation */
		bool						mIntersectionTest;				/**< Flag that indicates if we should be looking for intersections */
		bool						mObjectIntersected;				/**< The scene must set this flag in order to tell the manipulation method that an object has been intersected */
		bool						mManipulation;					/**< The manipulation method tells the scene that a transformation should be applied to the selected object */
		bool						mScale;							/**< A scaling manipulation method alerts the scene that a scaling has to be applied to the selected object */

		gmtl::Vec3f					mRayStart;						/**< Starting point of the casted ray, it is set by a manipulation method */
		gmtl::Vec3f					mRayEnd;						/**< End point for the casted ray, set by a manipulation method */

		gmtl::Matrix44f				mNavigationMatrix;				/**< The user's navigation matrix */
		gmtl::Matrix44f				mSelectedObjectTransformation;	/**< The transformation that will be applied to the selected object */
		gmtl::Matrix44f				mSelectedObjectScale;			/**< The scaling that will be applied to the selected object */

		vpr::Guard<vpr::Mutex>		*mGuard;						/**< Used to handle VR Juggler's mutex objects */
		vpr::Mutex					mLock;							/**< VR Juggler's mutex lock object */
	};
}
