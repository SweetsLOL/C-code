#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/**
 * On click button, the button text should change from "Click me" to "Clicked" vice versa
 */

 int x_rect = 50;
 int y_rect = 50;
 int w_rect= 100;
 int h_rect = 50;

void draw_button(const char *text, SDL_Renderer *renderer, TTF_Font *font, SDL_Color color)
{
	SDL_Rect btnRect;
	btnRect.x = x_rect;
	btnRect.y = y_rect;
	btnRect.w = w_rect;
	btnRect.h = h_rect;
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &btnRect);

	// Text in wrong position
	SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	int texW = 0, texH = 0;
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = {btnRect.x, btnRect.y + 10, texW, texH};

	

	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

void button_border(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color)
{
	SDL_Rect btnBorder;
	btnBorder.x = x_rect;
	btnBorder.y = y_rect;
	btnBorder.w = w_rect;
	btnBorder.h = h_rect;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &btnBorder);
}

void mouse_coords_text(SDL_Renderer *renderer, TTF_Font* font, SDL_Color color) {
	int x_coord;
	int y_coord;
	SDL_GetMouseState(&x_coord, &y_coord);
	char coord[256];
	sprintf(coord, "x=%d, y=%d", x_coord, y_coord);  

	SDL_Surface *surface = TTF_RenderText_Solid(font, coord, color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

	int texW = 0, texH = 0;
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

	SDL_Rect dstrect = { 0, 250, texW, texH };
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
	SDL_Color color = {255, 0, 0};

	// Application state
	bool isRunning = true;
	SDL_Event event;

	//Animation state, should be outside the 	
	bool wasClicked = false;
	
	
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
					printf("Mouse coordinate x - %d, y - %d\n", mouseX, mouseY);
					
					//if(mouseX < ...)
					// TODO: Complete this
					if(mouseX > x_rect && mouseX < (x_rect + 100) && mouseY > y_rect && mouseY < (y_rect + 50)){
						printf("The button was clicked. \n");
						wasClicked = true;
						x_rect = rand() % 200;
						y_rect = rand() % 250;
					}else{
						printf("The button was not clicked. \n");
						wasClicked = false;
					}

				}
				break;
			}
		}

		// Draw background
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		// Draw text
		if(wasClicked){
			draw_button("Clicked", renderer, font, color);
		}
		else
			draw_button("Click me", renderer, font, color);

		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		bool hovering = false;
	
		if(mouseX > x_rect && mouseX < (x_rect + 100) && mouseY > y_rect && mouseY < (y_rect + 50))
			hovering = true;
		else
			hovering = false;

		if(hovering){
			button_border(renderer, font, color);
		}


		mouse_coords_text(renderer, font, color);

		// Present Render to screen
		SDL_RenderPresent(renderer);

		SDL_Delay(10); // Delay 100 ms

		
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();

	return 0;
}
