#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void draw_text(const char *text, SDL_Renderer *renderer, TTF_Font *font, SDL_Color color)
{
	SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	int texW = 0, texH = 0;
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = {0, 0, texW, texH};
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

int main(int argc, char *args[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	SDL_Window *window = SDL_CreateWindow(
		"Task 4",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 300, 0);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	// Text Related
	TTF_Font *font = TTF_OpenFont("arial.ttf", 25);
	SDL_Color color = {255, 255, 255}; //white

	// Application state
	uint64_t frame = 0;
	bool isRunning = true;
	SDL_Event event;

	while (isRunning)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			// Handles OS "Exit" event
			case SDL_QUIT:
				isRunning = false;
				break;
			}
		}

		// Draw black background
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		/************/
		// TODO: Draw Something here
		//change colour before drawing the next shape/item
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderDrawLine(renderer, 50, 50, 250, 250);
		SDL_RenderDrawLine(renderer, 250, 50, 50, 250);
		//Draw rectangle - outline only
		SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
		SDL_Rect rect1 = {100, 100, 100, 100};
		SDL_RenderDrawRect(renderer, &rect1); //just outline
		//Draw rectangle - with colour
		SDL_SetRenderDrawColor(renderer, 0, 100, 100, 255);
		SDL_Rect rect2 = {50, 100, 25, 100};
		SDL_RenderFillRect(renderer, &rect2); //will have colour
		//Draw triangle with lines
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, 0, 220, 150, 0);
		SDL_RenderDrawLine(renderer, 300, 220, 150, 0);
		SDL_RenderDrawLine(renderer, 0, 220, 300, 220);
		//Draw triangle with vertices (SDL_Vertex)
		SDL_Vertex vertex1 = {{50.0, 280.0}, {255, 0, 0, 255}, {1, 1}};
		SDL_Vertex vertex2 = {{250.0, 280.0}, {0, 255, 0, 255}, {1, 1}};
		SDL_Vertex vertex3 = {{150.0, 200.0}, {0, 0, 255, 255}, {1, 1}};
		//Put the vertices into an array
		SDL_Vertex vertices[] = {
			vertex1,
			vertex2,
			vertex3
		};
		SDL_RenderGeometry(renderer, NULL, vertices, 3, NULL, 0);
		/************/

		// Draw text
		char text[16];
		sprintf(text, "Frame %d", frame % 1000);
		draw_text(text, renderer, font, color);
		frame += 1;

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
