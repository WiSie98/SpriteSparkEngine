#include <SpriteSpark.h>

class TestApp : public SpriteSpark::Application {

public:
    TestApp() { }

    ~TestApp() { }
};

SpriteSpark::Application* SpriteSpark::CreateApplication() {
    return new TestApp();
}