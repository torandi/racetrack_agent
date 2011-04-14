#include <cstdlib>
#include <cstdio>
#include <pthread.h>
#include <GL/glfw.h>
#include <string>

#include "decision_module.h"
#include "genetic_algorithm.h"
#include "graphics.h"
#include "config.h"

void GLFWCALL DMFunction(void *arg) {
    DecisionModule * dm = (DecisionModule *)arg;
	dm->run();
	delete dm;
}

int main(int argc,const char * argv[]) {
	 std::string track="oval.lvl"; //default level

	 if(argc>1) {
		track=std::string(argv[1]);
	 }

	 Config config=Config();
	if(!config.open(track)) {
		printf("Open error\n");
		exit(1);
	}

    Graphics::init(800, 600);

    // Create the thread for the decision module.
    DecisionModule * dm = new GeneticAlgorithm(config);
    glfwCreateThread(DMFunction, dm);

    Graphics::drawLoop();

    glfwTerminate();

    return 0;
}

