/**
 * An SDL app that prints the video information of the current host
 */
#include "SDL_video.h"
#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>

int main(void)
{
#ifndef SDL_NEXT
	if (SDL_InitSubSystem(SDL_INIT_EVERYTHING) < 0)
		SDL_Log("SDL fails to initialize! %s\n", SDL_GetError());

	int num_drivers = SDL_GetNumVideoDrivers();
	printf("Number of drivers: %d\n", num_drivers);
	for (int i = 0; i < num_drivers; i++)
		printf("Driver name: %s\n", SDL_GetVideoDriver(i));

	int num_displays = SDL_GetNumVideoDisplays();
	printf("Number of displays: %d\n", num_displays);
	for (int d = 0; d < num_displays; d++) {
		int num_modes = SDL_GetNumDisplayModes(d);
		printf("Display %d, modes: %d", d, num_modes);
		for (int m = 0; m < num_modes; m++) {
			SDL_DisplayMode mode;
			SDL_GetDisplayMode(d, m, &mode);
			printf("\tMode(%d) format: %d, %dx%d %dHz\n", m, mode.format, mode.w, mode.h, mode.refresh_rate);
		}
	}

	SDL_Window *window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
	if (!window) {
		printf("Failed to create window: %s\n", SDL_GetError());
	}

	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	printf("Window dimensions: %dx%d\n", w, h);

	bool quit = false;
	SDL_Event e;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}
	}

	SDL_DestroyWindow(window);

	return 0;
#endif
}
