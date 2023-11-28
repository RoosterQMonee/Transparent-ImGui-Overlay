#include <iostream>

#include "Overlay/Overlay.h"


void Draw(void)
{
	// ImGui custom things
	ImGui::Text("This took me 2 hours.");
}


int main()
{
	Overlay window = Overlay(&Draw, 1920, 1080); // note: The width / height are the dimensions of the overlay WINDOW not the overlay itself.

	window.Start();
}