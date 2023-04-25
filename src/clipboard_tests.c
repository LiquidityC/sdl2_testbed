#include <stdio.h>
#include <stdbool.h>

#include "SDL.h"

#define BUF_SIZE 1024*1024

#ifdef SDL_LOCAL
static SDL_Texture *texture_from_clipboard(SDL_Renderer *renderer, SDL_FRect *size)
{
	size_t length;
	void *data = SDL_GetClipboardData(&length, "image/bmp");
	SDL_RWops *rw;
	SDL_Surface *surface;
	SDL_Texture *texture = NULL;

	if (data != NULL && length > 0) {
		rw = SDL_RWFromMem(data, length);
		surface = SDL_LoadBMP_RW(rw, 1);
		size->w = surface->w;
		size->h = surface->h;
		texture = SDL_CreateTextureFromSurface(renderer, surface);

		SDL_DestroySurface(surface);
		SDL_free(data);
	} else {
		printf("Size: %zu\n", length);
	}

	return texture;
}

static const char *mime_types[] = {
	"image/jpeg",
	"image/bmp",
	NULL
};

void *callback_func(size_t *length, const char *mime_type, void *userdata)
{
	Uint8 buf[BUF_SIZE];
	void *data = NULL;
	FILE *fp;
	*length = 0;

	printf("Mime-type: %s requested\n", mime_type);
	if (SDL_strcmp(mime_type, "image/jpeg") == 0) {
		puts("Providing jpeg");
		fp = fopen("../assets/small_panda.jpeg", "rb");
	} else {
		puts("Providing bmp");
		fp = fopen("../assets/small_panda.bmp", "rb");
	}

	if (fp != NULL) {
		*length = fread(buf, 1, BUF_SIZE, fp);
		fclose(fp);

		data = SDL_malloc(*length);
		SDL_memcpy(data, buf, *length);
	}


	return data;
}

#endif

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
	SDL_Texture *texture = NULL;
	SDL_FRect size = { 0, 0, 0, 0 };
	while (running) {
		bool update = false;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_EVENT_QUIT:
					running = SDL_FALSE;
					break;
				case SDL_EVENT_CLIPBOARD_UPDATE: {
					puts("Clipboard update");
					update = true;
					if (SDL_HasClipboardText()) {
						char *content = SDL_GetClipboardText();
						printf("Text: %s\n", content);
						SDL_free(content);
					}
					if (SDL_HasPrimarySelectionText()) {
						char *content = SDL_GetPrimarySelectionText();
						printf("Selection: %s\n", content);
						SDL_free(content);
					}
				} break;
				case SDL_EVENT_CLIPBOARD_CANCELLED:
					puts("Clipboard cancelled");
					SDL_free(e.clipboard.userdata);
					break;
				case SDL_EVENT_KEY_DOWN:
					if (e.key.keysym.sym == SDLK_c) {
						SDL_SetClipboardData(callback_func, mime_types, SDL_malloc(100));
					} else if (e.key.keysym.sym == SDLK_t)
						SDL_SetClipboardText("I'm batman");
					else if (e.key.keysym.sym == SDLK_s)
						SDL_SetPrimarySelectionText("I'm batman");
					break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		SDL_RenderClear(renderer);

		if (update) {
			if (SDL_HasClipboardData("image/bmp")) {
				printf("[%lu]: Clipboard contains image/bmp\n", SDL_GetTicks());
				if (texture != NULL) {
					SDL_DestroyTexture(texture);
					texture = NULL;
				}
				texture = texture_from_clipboard(renderer, &size);
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

	void *data = NULL;

clean:

	data = SDL_GetClipboardUserdata();
	if (data != NULL) {
		puts("Cleaning up userdata");
		SDL_free(data);
	} else {
		puts("No userdata to clean");
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
out:
#endif
	return result;
}
