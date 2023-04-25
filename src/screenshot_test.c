#ifdef SDL_NEXT

#include <stdio.h>
#include "SDL.h"

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

	SDL_CreateWindowAndRenderer(400, 300, 0, &window, &renderer);
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
	while (running) {
		SDL_bool take_screenshot = SDL_FALSE;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_EVENT_QUIT:
					running = SDL_FALSE;
					break;
				case SDL_EVENT_KEY_DOWN:
					if (e.key.keysym.sym == SDLK_p)
						take_screenshot = SDL_TRUE;
					break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

		/* Draw some shapes */
		SDL_FRect r = { 20, 20, 100, 100 };
		SDL_RenderFillRect(renderer, &r);

		r.x += 40;
		r.y += 40;
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderRect(renderer, &r);

		r.x += 40;
		r.y += 40;
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderRect(renderer, &r);

		if (take_screenshot == SDL_TRUE) {
			puts("Creating screenshot");
			SDL_Rect vp;
			SDL_RendererInfo info;
			SDL_GetRendererInfo(renderer, &info);
			SDL_assert(SDL_GetRenderViewport(renderer, &vp) == 0);
			printf("Driver: %s, Viewport dimension: %dx%d\n", info.name, vp.w, vp.h);
			SDL_Surface *sshot = SDL_CreateSurface(vp.w, vp.h, SDL_PIXELFORMAT_ARGB8888);
			SDL_assert(SDL_RenderReadPixels(renderer, NULL, sshot->format->format, sshot->pixels, sshot->pitch) == 0);
			SDL_assert(SDL_SaveBMP(sshot, "screenshot.bmp") == 0);
			SDL_DestroySurface(sshot);
		}

		SDL_RenderPresent(renderer);
	}

clean:
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
out:
	return result;
}

#else
int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	return 0;
}
#endif
