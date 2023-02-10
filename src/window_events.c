/**
 * An SDL app used for testing SDL_WINDOWEVENT occurences
 */
#include <stdio.h>

#include "SDL.h"
#include "SDL_events.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "SDL_video.h"

#ifdef SDL_NEXT
#define SDL_WINDOWEVENT_ENTER SDL_EVENT_WINDOW_MOUSE_ENTER
#define SDL_WINDOWEVENT_LEAVE SDL_EVENT_WINDOW_MOUSE_LEAVE
#define SDL_WINDOWEVENT_FOCUS_GAINED SDL_EVENT_WINDOW_FOCUS_GAINED
#define SDL_WINDOWEVENT_FOCUS_LOST SDL_EVENT_WINDOW_FOCUS_LOST
#define SDL_WINDOWEVENT SDL_EVENT_WINDOW
#define SDL_DROPFILE SDL_EVENT_DROP_FILE
#define SDL_QUIT SDL_EVENT_QUIT

#define PRINT_MOUSE_STATE(action)                                                                                      \
	do {                                                                                                           \
		float x, y;                                                                                            \
		Uint32 mask = SDL_GetMouseState(&x, &y);                                                               \
		printf("%s: %.2fx%.2f (%u)\n", action, x, y, mask);                                                    \
	} while (0);

#else
#define PRINT_MOUSE_STATE(action)                                                                                      \
	do {                                                                                                           \
		int x, y;                                                                                              \
		Uint32 mask = SDL_GetMouseState(&x, &y);                                                               \
		printf("%s: %dx%d (%u)\n", action, x, y, mask);                                                        \
	} while (0);
#endif

static void
parse_window_events(SDL_Event *event)
{
#ifndef SDL_NEXT
	switch (event->window.event) {
#else
	switch (event->type) {
#endif
		case SDL_WINDOWEVENT_ENTER:
			PRINT_MOUSE_STATE("Mouse enter");
			break;
		case SDL_WINDOWEVENT_LEAVE:
			PRINT_MOUSE_STATE("Mouse leave");
			break;
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			PRINT_MOUSE_STATE("Focus gained");
			break;
		case SDL_WINDOWEVENT_FOCUS_LOST:
			PRINT_MOUSE_STATE("Focus lost");
			break;
		default:
			break;
	}
}

static void
parse_events(SDL_Event *event)
{
	switch (event->type) {
#ifndef SDL_NEXT
		case SDL_WINDOWEVENT: {
			parse_window_events(event);
		} break;
#endif
		case SDL_DROPFILE: {
			printf("Drop event!\n");
		} break;
		default:
			break;
	}
#ifdef SDL_NEXT
	parse_window_events(event);
#endif
}

int
main(int argc, char **argv)
{
	int result = 0;
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("Err: %s\n", SDL_GetError());
		result = -1;
		goto out;
	}

	window = SDL_CreateWindow("Testbed", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	if (!window) {
		printf("Err: %s\n", SDL_GetError());
		result = -1;
		goto clean;
	}

	renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		printf("Err: %s\n", SDL_GetError());
		result = -1;
		goto clean;
	}

	/* Main loop */
	SDL_Event e;
	SDL_bool running = SDL_TRUE;
	while (running) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_QUIT:
					running = SDL_FALSE;
					break;
			}
			parse_events(&e);
		}

		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		SDL_RenderPresent(renderer);
	}

clean:
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
out:
	return result;
}
