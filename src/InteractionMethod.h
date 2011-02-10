#pragma once

namespace vis {

	class InputHandler;

	class InteractionMethod {
	public:
		InteractionMethod(unsigned int buttonNumber = 0){
			mButtonNumber = buttonNumber;
		}

		~InteractionMethod(){

		}

		void init(InputHandler* input){
			mInput = input;
		}

		void update(){

		}

	protected:
		unsigned int mButtonNumber;
		InputHandler* mInput;
	};
}
