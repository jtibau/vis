#include "App.h"
#include "Scene.h"
#include "InputHandler.h"
#include "InteractionMethod.h"

namespace vis {
	App::App(vrj::Kernel* kern) : vrj::opengl::App(kern) {
		mInput = new InputHandler();
		mInteraction = new InteractionMethod();
		mScene = new Scene();
	}

	App::~App() {
		delete mInput;
		delete mScene;
		delete mInteraction;
	}

	void App::init() {
		vrj::opengl::App::init();
		mInput->init();

		mInteraction->init(mInput);
		mScene->init(mInput);
	}

	void App::contextInit() {
		mScene->contextInit();
	}

	void App::preFrame() {
		mInput->update();
		mInteraction->update();
	}

	void App::latePreFrame() {
		mScene->latePreFrame();
	}

	void App::bufferPreDraw() {
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void App::draw() {
		glClear(GL_DEPTH_BUFFER_BIT);

		// Users have reported problems with OpenGL reporting stack underflow
		// problems when the texture attribute bit is pushed here, so we push all
		// attributes *except* GL_TEXTURE_BIT.
		glPushAttrib(GL_ALL_ATTRIB_BITS & ~GL_TEXTURE_BIT);
		glPushAttrib(GL_TRANSFORM_BIT);
		glPushAttrib(GL_VIEWPORT_BIT);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();

		mScene->draw();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glPopAttrib();
		glPopAttrib();
		glPopAttrib();

		// Drawing the Wand
		glEnable(GL_DEPTH_TEST);
		glColor3f(0.663f,0.83f,0.51f);
		glLineWidth(2.);

		glBegin(GL_LINES);
		{
			gmtl::Vec3f start	= mInput->rayStart();
			gmtl::Vec3f end		= mInput->rayEnd();

			glVertex3f(start[0],start[1],start[2]);
			glVertex3f(end[0],end[1],end[2]);
		}
		glEnd();
	}


	//void App::exit() {}

	float App::getDrawScaleFactor() {
		return gadget::PositionUnitConversion::ConvertToMeters;
	}
}
