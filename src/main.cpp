#include "App.h"

#include <vrj/vrjConfig.h>
#include <vrj/Kernel/Kernel.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

int main(int argc, char* argv[]){
	if(argc < 2){
		std::cout << "Usage: " << argv[0] << " configFile.jconf\n";
		std::exit(EXIT_FAILURE);
	}

	vrj::Kernel* kernel = vrj::Kernel::instance();
	vis::App* application = new vis::App(kernel);

	kernel->init(argc,argv);
	kernel->loadConfigFile(argv[1]);

	kernel->start();
	kernel->setApplication(application);

	kernel->waitForKernelStop();
	kernel->stop();

	return EXIT_SUCCESS;
}
