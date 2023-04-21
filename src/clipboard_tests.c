#include <stdio.h>

#include "SDL.h"
#include "SDL_events.h"

int main(int argc, char **argv)
{
	int result = 0;
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("Err: %s\n", SDL_GetError());
		result = -1;
		goto out;
	}

	SDL_SetEventEnabled(SDL_EVENT_CLIPBOARD_DATA_UPDATE, SDL_TRUE);

	window = SDL_CreateWindow("Testbed", 800, 600, 0);
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
				case SDL_EVENT_QUIT:
					running = SDL_FALSE;
					break;
				case SDL_EVENT_CLIPBOARD_UPDATE:
					puts("Clipboard update");
					break;
				case SDL_EVENT_CLIPBOARD_DATA_UPDATE:
					puts("Clipboard data update");
					printf("Event: %s\n", e.clipboard.mime_type);
					SDL_free(e.clipboard.mime_type);
					break;
			}
		}

		if (SDL_HasClipboardData("image/png")) {
			printf("[%lu]: Clipboard contains image/png\n", SDL_GetTicks());
		} else {
			printf("[%lu]: Clipboard does not contain image/png\n", SDL_GetTicks());
		}
		printf("SDL_Event size: %zu\n", sizeof(SDL_Event));

		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		SDL_RenderPresent(renderer);
		SDL_Delay(1000);
	}

clean:
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
out:
	return result;
}
