// GraphicsPlayground.cpp : Defines the entry point for the application.
//

#include "GraphicsPlayground.h"
#include "App.h"
#include "Graphics.h"
#include "WindowSettings.h"

#include <exception>

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

    try
    {
        gfx.Init();
        gfx.PrepForWindow(app);

        bool running = false;
        do
        {
            running = app.Update();
            if (running)
            {
                gfx.Update();
                gfx.Draw();
            }
        } while (running);
    }
    catch(std::exception&)
    {
        // TODO: handle me
    }
    gfx.Shutdown();

    return 0;
}


