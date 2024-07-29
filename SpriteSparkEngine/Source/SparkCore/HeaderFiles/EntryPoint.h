#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>

extern SpriteSpark::Application* SpriteSpark::CreateApplication();

int main(int argc, char** argv) {

    auto app = SpriteSpark::CreateApplication();
    app->Run();
    delete app;

    return EXIT_SUCCESS;
}