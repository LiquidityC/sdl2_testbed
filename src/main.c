#include "SDL.h"
#include "SDL_events.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "SDL_video.h"

#define PRINT_MOUSE_STATE(action)                                                                                      \
	do {                                                                                                           \
		int x, y;                                                                                              \
		Uint32 mask = SDL_GetMouseState(&x, &y);                                                               \
		printf("%s: %dx%d (%u)\n", action, x, y, mask);                                                        \
	} while (0);

static void
parse_window_events(SDL_Event *event)
{
	switch (event->window.event) {
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
			PRINT_MOUSE_STATE("Focus gained");
			break;
		default:
			break;
	}
}

static void
parse_events(SDL_Event *event)
{
	switch (event->type) {
		case SDL_WINDOWEVENT: {
			parse_window_events(event);
		} break;
		case SDL_DROPFILE: {
			printf("Drop event!\n");
		} break;
		default:
			break;
	}
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
		goto clean;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		printf("Err: %s\n", SDL_GetError());
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
