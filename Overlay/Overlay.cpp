#include "Overlay.h"


LPWSTR WindClass;
LPWSTR WindName;


LPWSTR cctow(const char* str)
{
	int wcharLength = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	wchar_t* wideString = new wchar_t[wcharLength];

	MultiByteToWideChar(CP_UTF8, 0, str, -1, wideString, wcharLength);

	return wideString;
}


Overlay::Overlay(void (*render_function)(void), int WindowWidth, int WindowHeight, bool displayFPS,
				const char* OverlayTitle, const char* WindowClass, const char* WindowName)
{
	this->render_function = render_function;
	this->title = OverlayTitle;
	this->showFPS = displayFPS;
	this->initialized = true;

	Setup(render_function, WindowWidth, WindowHeight);

	WindClass = cctow(WindowClass);
	WindName = cctow(WindowName);
}

Overlay::~Overlay()
{
	if (!this->initialized)
	{
		delete[] WindClass;
		delete[] WindName;

		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();

		ImGui::DestroyContext();

		CleanupDeviceD3D();
		DestroyWindow(hwnd);
	}
}


void Overlay::Start()
{
	StartOverlay(WindClass, WindName, this->title, this->showFPS);
}
