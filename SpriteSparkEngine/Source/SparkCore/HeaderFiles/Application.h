#pragma once

namespace SpriteSpark {

	class Application {

	public:

		Application();
		virtual ~Application();

		void Run();

	private:

	};

    Application* CreateApplication();

}