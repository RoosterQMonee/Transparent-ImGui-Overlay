#pragma once

#include <memory>

#include "Utils/Window.h"


class Overlay
{
	private:
		void (*render_function)(void);
		bool initialized;
		bool showFPS;

		const char* title;

	public:
		Overlay(void (*render_function)(void),
				int WindowWidth, int WindowHeight,
				const char* OverlayTitle = "Overlay",
				const char* WindowClass = "OverlayClass",
				const char* WindowName = "Windowless Overlay",
				bool displayFPS = true);
		~Overlay();

		void Start();
};