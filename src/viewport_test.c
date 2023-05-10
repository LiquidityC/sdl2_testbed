#include "SDL.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	int result = 0;
#ifdef SDL_LOCAL
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("Err: %s\n", SDL_GetError());
		result = -1;
		goto out;
	}

	SDL_CreateWindowAndRenderer(800, 600, 0, &window, &renderer);
	if (!window) {
		printf("Err: %s\n", SDL_GetError());
		result = -1;
		goto clean;
	}

	if (!renderer) {
		printf("Err: %s\n", SDL_GetError());
		result = -1;
		goto clean;
	}

	/* Main loop */
	SDL_Event e;
	SDL_bool running = SDL_TRUE;
	SDL_Rect vp_rect = { 100, 100, 32, 32 };

	while (running) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_EVENT_QUIT:
					running = SDL_FALSE;
					break;
				case SDL_EVENT_KEY_DOWN:
					if (e.key.keysym.sym == SDLK_v) {
						SDL_SetRenderViewport(renderer, &vp_rect);
					}
					break;
				case SDL_EVENT_KEY_UP:
					if (e.key.keysym.sym == SDLK_v) {
						SDL_SetRenderViewport(renderer, NULL);
					}
					break;
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_Rect r;
		SDL_GetRenderViewport(renderer, &r);
		// printf("Viewport: %d %d %d %d\n", r.x, r.y, r.w, r.h);
		SDL_FRect s = { 2, 2, r.w - 2, r.h - 2 };
		SDL_RenderRect(renderer, &s);

		SDL_RenderPresent(renderer);
		SDL_Delay(100);
	}

clean:
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

out:
#endif
	return result;
}
