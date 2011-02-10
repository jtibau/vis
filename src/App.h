#pragma once

#include <vrj/vrjConfig.h>
#include <vrj/Draw/OpenGL/App.h>
#include <vrj/Kernel/Kernel.h>

namespace vis {

	class Scene;
	class InputHandler;
	class InteractionMethod;

	class App : public vrj::opengl::App {

	public:
		App(vrj::Kernel* kern);
		~App();

		void init();
		void contextInit();
		void preFrame();
		void latePreFrame();
		void bufferPreDraw();
		void draw();

		//void exit();

		float getDrawScaleFactor();

	protected:
		Scene* mScene;
		InputHandler* mInput;
		InteractionMethod* mInteraction;
	};

}
