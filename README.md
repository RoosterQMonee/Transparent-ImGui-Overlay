# Transparent-ImGui-Overlay

Transparent overlay thing

## Overview

cool transparent overlay thingy i made in like 2 hours. i made it to make menus easier.

## How to use:

```cpp
#include "Overlay/Overlay.h" // include header

void Draw(void) // create rendering function (no ::Begin or ::End required)
{
	// ImGui custom things
	ImGui::Text("This took me 2 hours.");
}


int main()
{
  //                       Func   W     H      <-  W / H are the Window dimensions, not overlay.
	Overlay window = Overlay(&Draw, 1920, 1080); // Create Overlay

	window.Start(); // start overlay (starts a loop, you may want to use a thread to continue in main)
}
```

### Notes:

There were a couple of strange issues with compiling that made the code look a bit weird, if you know how to fix this, please create an issue or pull request :)
