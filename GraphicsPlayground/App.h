#pragma once
#include "framework.h"
#include "WindowSettings.h"

class App final
{
public:

	void Init(HINSTANCE ahInstance, int anCmdShow);
	void MakeWindow(const WindowSettings& arSettings);
	bool Update();

	HWND GetWnd() const { return hWnd; }
	const WindowSettings& GetSettings() const { return Settings; }
private:
	HINSTANCE hInst;
	int nCmdShow;
	HWND hWnd;

	static const int MaxStringC = 100;

	WCHAR szTitle[MaxStringC];			 // The title bar text
	WCHAR szWindowClass[MaxStringC];	 // the main window class name
	WindowSettings Settings;

};

