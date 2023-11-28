#pragma once

#include "../../Dependencies/ImGui/imgui.h"
#include "../../Dependencies/ImGui/imgui_impl_dx9.h"
#include "../../Dependencies/ImGui/imgui_impl_win32.h"

#include "Device.h"

#include <thread>
#include <chrono>


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern void StartOverlay(LPCWSTR AppClass, LPCWSTR AppName, const char* title, bool fps);
extern void Setup(void (*df)(void), int ww, int wh);