#include <stdio.h>
#include <stdbool.h>

#include "SDL.h"
#include "SDL_clipboard.h"
#include "SDL_events.h"
#include "SDL_oldnames.h"
#include "SDL_pixels.h"
#include "SDL_render.h"
#include "SDL_rwops.h"
#include "SDL_surface.h"
#include "SDL_video.h"

#define BUF_SIZE 1024*1024

static SDL_Texture *texture_from_clipboard(SDL_Renderer *renderer, SDL_FRect *size)
{
	size_t length;
	void *data = SDL_GetClipboardData(&length, "image/bmp");
	SDL_RWops *rw;
	SDL_Surface *surface;
	SDL_Texture *texture;

	rw = SDL_RWFromMem(data, length);
	surface = SDL_LoadBMP_RW(rw, 1);
	size->w = surface->w;
	size->h = surface->h;
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_DestroySurface(surface);
	SDL_free(data);

	return texture;
}

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
	SDL_Texture *texture = NULL;
	SDL_FRect size = { 0, 0, 0, 0 };
	while (running) {
		bool update = false;
		bool clipboard = false;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_EVENT_QUIT:
					running = SDL_FALSE;
					break;
				case SDL_EVENT_CLIPBOARD_UPDATE:
					puts("Clipboard update");
					update = true;
					break;
				case SDL_EVENT_KEY_DOWN:
					if (e.key.keysym.sym == SDLK_c)
						clipboard = true;
					break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		SDL_RenderClear(renderer);

		if (update) {
			if (SDL_HasClipboardData("image/bmp")) {
				printf("[%lu]: Clipboard contains image/png\n", SDL_GetTicks());
				if (texture != NULL) {
					SDL_DestroyTexture(texture);
					texture = NULL;
				}
				texture = texture_from_clipboard(renderer, &size);
			}
		}

		if (clipboard) {
			printf("Pushing to clipboard\n");

			Uint8 bmp_data[BUF_SIZE];
			Uint8 jpg_data[BUF_SIZE];
			size_t jpg_size= 0, bmp_size = 0;
			FILE *fp;

			fp = fopen("../assets/small_panda.bmp", "rb");
			if (fp != NULL) {
				printf("Including bmp\n");
				bmp_size = fread(bmp_data, 1, BUF_SIZE, fp);
				fclose(fp);
			}
			fp = fopen("../assets/small_panda.jpeg", "rb");
			if (fp != NULL) {
				printf("Including jpeg\n");
				jpg_size = fread(jpg_data, 1, BUF_SIZE, fp);
				fclose(fp);
			}

			SDL_ClipboardData *cb = SDL_CreateClipboardData(bmp_data, bmp_size, "image/bmp");
			SDL_AddClipboardData(cb, jpg_data, jpg_size, "image/jpeg");

			if (SDL_SetClipboardData(cb, SDL_TRUE) != 0) {
				printf("ERR: %s\n", SDL_GetError());
			}
		}

		if (texture != NULL) {
			SDL_RenderTexture(renderer, texture, NULL, &size);
		}

		SDL_FRect r = { 20, 20, 760, 560 };
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderRect(renderer, &r);

		SDL_RenderPresent(renderer);
	}

clean:
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
out:
	return result;
}
