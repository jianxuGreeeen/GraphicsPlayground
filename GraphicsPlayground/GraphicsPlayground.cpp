// GraphicsPlayground.cpp : Defines the entry point for the application.
//
#pragma warning(disable : 26812)

#include "GraphicsPlayground.h"
#include "App.h"
#include "GameGraphics.h"
#include "Graphics.h"
#include "WindowSettings.h"

#include <exception>
#include <memory>
#include <iostream>

WindowSettings LoadSettings()
{
    WindowSettings settings;
    settings.Width = 1280;
    settings.Height = 720;
    return settings;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    auto wndSettings = LoadSettings();

    App app;
    app.Init(hInstance, nCmdShow);
    app.MakeWindow(wndSettings);

    Graphics gfx;
    GameGraphics gameGfx;
    try
    {
        gfx.Init();
        gfx.PrepForWindow(app);
        gfx.LoadResources();

        gameGfx.Init(app, gfx);

        bool running = false;
        do
        {
            running = app.Update();
            if (running)
            {
                gfx.Update();
                gameGfx.Update(app, gfx);
                gfx.Draw();
            }
        } while (running);
    }
    catch(std::exception& e)
    {
        // TODO: handle me
        std::cout << e.what() << std::endl;
    }

    gameGfx.Shutdown(app, gfx);
    gfx.Shutdown();
    return 0;
}


