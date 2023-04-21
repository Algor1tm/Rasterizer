#pragma once

#include "Core/Core.h"
#include "Core/Application.h"


extern Core::Application* Core::CreateApplication();

#ifndef APP_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
    Core::Application* app = Core::CreateApplication();

    app->Run();
    delete app;

    return EXIT_SUCCESS;
}
#else

#include <Windows.h>


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)

{
    Core::Application* app = Core::CreateApplication();

    app->Run();
    delete app;

    return EXIT_SUCCESS;
}

#endif
