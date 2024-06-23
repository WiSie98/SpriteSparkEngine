#pragma once

extern SpriteSpark::Application* SpriteSpark::CreateApplication();

int main(int argc, char** argv) {

    auto app = SpriteSpark::CreateApplication();
    app->Run();
    delete app;

    return 0;
}