#pragma once
#include "framework.h"
#include "WindowSettings.h"

class App final
{
public:

	void Init(HINSTANCE ahInstance);
	void MakeWindow(const WindowSettings& arSettings);
	bool Update();

	HWND GetWnd() const { return hWnd; }
	const WindowSettings& GetSettings() const { return Settings; }
	float GetWidth() const;
	float GetHeight() const;

private:
	HINSTANCE hInst;
	HWND hWnd;

	static const int MaxStringC = 100;

	WCHAR szTitle[MaxStringC];			 // The title bar text
	WCHAR szWindowClass[MaxStringC];	 // the main window class name
	WindowSettings Settings;
	RECT ClientRect;

	void UpdateRect();
};

