#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/**
 * On click button, the button text should change from "Click me" to "Clicked" vice versa
 */

void draw_button(const char *text, SDL_Renderer *renderer, TTF_Font *font, SDL_Color color)
{
	// Text in wrong position
	SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	int texW = 0, texH = 0;
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = {0, 0, texW, texH};

	SDL_Rect btnRect;
	btnRect.x = 50;
	btnRect.y = 50;
	btnRect.w = 100;
	btnRect.h = 50;
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &btnRect);

	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

int main(int argc, char *args[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	SDL_Window *window = SDL_CreateWindow(
		"Task 5",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 300, 0);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	// Text Related
	TTF_Font *font = TTF_OpenFont("arial.ttf", 25);
	SDL_Color color = {255, 255, 255};

	// Application state
	bool isRunning = true;
	SDL_Event event;

	while (isRunning)
	{
		if (SDL_PollEvent(&event) > 0)
		{
			switch (event.type)
			{
			// Handles OS "Exit" event
			case SDL_QUIT:
				isRunning = false;
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					int mouseX = event.motion.x;
    				int mouseY = event.motion.y;
					
					if(mouseX < ...)
					// TODO: Complete this
				}
				break;
			}
		}

		// Draw black background
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Draw text
		char text[16];
		draw_button("Click me", renderer, font, color);

		// Present Render to screen
		SDL_RenderPresent(renderer);

		SDL_Delay(100); // Delay 100 ms
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();

	return 0;
}
