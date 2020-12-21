#include "App.h"
#include "Resource.h"

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

// TODO error handling
void App::Init(HINSTANCE ahInstance)
{
    hInst = ahInstance;

    LoadStringW(ahInstance, IDS_APP_TITLE, szTitle, MaxStringC);
    LoadStringW(ahInstance, IDC_GRAPHICSPLAYGROUND, szWindowClass, MaxStringC);

    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = ahInstance;
    wcex.hIcon = LoadIcon(ahInstance, MAKEINTRESOURCE(IDI_GRAPHICSPLAYGROUND));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);
}

// TODO support additional settings like full screen
void App::MakeWindow(const WindowSettings& arSettings)
{
    Settings = arSettings;

    HWND hWndLocal = CreateWindowEx(NULL, 
        szWindowClass, szTitle, 
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 
        arSettings.Width, arSettings.Height,
        nullptr, nullptr, hInst, nullptr);

    if (hWndLocal)
    {
        ShowWindow(hWndLocal, SW_SHOW);
        UpdateWindow(hWndLocal);
        hWnd = hWndLocal;
    }
}

bool App::Update()
{
    bool running = true;
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        running = msg.message != WM_QUIT;
        if (running)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    UpdateRect();
    
    return running;
}

float App::GetWidth() const 
{ 
    return static_cast<float>(ClientRect.right - ClientRect.left);
}

float App::GetHeight() const 
{ 
    return static_cast<float>(ClientRect.bottom - ClientRect.top);
}

void App::UpdateRect()
{
    GetClientRect(hWnd, &ClientRect);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
