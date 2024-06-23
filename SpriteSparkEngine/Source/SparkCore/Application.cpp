#include "HeaderFiles/Application.h"
#include "HeaderFiles/Log.h"

namespace SpriteSpark {

	Application::Application() { }

	Application::~Application() { }

	void Application::Run() {
        int a = 0;
        double f = 3.5;

		while (true) {
            SP_CORE_INFO("Test ", a, " Double: ", f);
            ++a;
		}
	}

}