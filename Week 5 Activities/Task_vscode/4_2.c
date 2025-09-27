#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdlib.h> //for rand()
#include <SDL2/SDL_ttf.h>

void draw_text(const char * text, SDL_Renderer *renderer, TTF_Font* font, SDL_Color color){
		SDL_Surface * surface = TTF_RenderText_Solid(font, text, color);
	    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    	int texW = 0, texH = 0;
    	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    	SDL_Rect dstrect = { 0, 0, texW, texH };
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
 		SDL_DestroyTexture(texture);
    	SDL_FreeSurface(surface);
}

int main(int argc, char* args[]) {
  	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	SDL_Window *window = SDL_CreateWindow(
        "Task 4",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 300, 0
    );

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	// Text Related
    TTF_Font * font = TTF_OpenFont("arial.ttf", 25);
    SDL_Color color = { 255, 255, 255 };

	// Application state
	uint64_t frame = 0;
	bool isRunning = true;
	SDL_Event event;

	int x = 0, y = 0;

	while (isRunning)
	{
		while (SDL_PollEvent(&event))
		{
            switch(event.type){
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
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_Rect random_rect = {x, y, 50, 50};
		SDL_RenderFillRect(renderer, &random_rect);

        /************/

		// Draw text
		char text[16];
		sprintf(text, "Frame %d", frame % 1000);
		draw_text(text, renderer, font, color);
		frame += 1;

        /************/
        // TODO: Animation Logic here
		x = rand() % 250; // 0 - 249
		y = rand() % 250; // 0 - 249
		//x = rand() % 300; // 0 - 299
        /************/

        // Present Render to screen
        SDL_RenderPresent(renderer);
        SDL_Delay(1); // Delay 100 ms
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
    TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();

	return 0;
}

