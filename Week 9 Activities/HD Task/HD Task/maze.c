#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>

#define CELL_SIZE 30
#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT 300
#define ROWS WINDOW_WIDTH / CELL_SIZE
#define COLS WINDOW_HEIGHT / CELL_SIZE

typedef struct cell
{
	char id[3];

	// have a pointer to the neighbouring cells
	struct cell *north;
	struct cell *south;
	struct cell *east;
	struct cell *west;

	// record whether this cell is wall or a path
	// default is a wall.
	bool wall;
	// this stops cycles - set when you travel through a cell
	bool visited;
	bool on_path;
} Cell;

void initialised_cell(Cell cells[][COLS])
{
	for (int r = 0; r < ROWS; r++)
	{
		for (int c = 0; c < COLS; c++)
		{
			sprintf(cells[r][c].id, "%d%d", r, c);
			cells[r][c].north = NULL;
			cells[r][c].south = NULL;
			cells[r][c].east = NULL;
			cells[r][c].west = NULL;

			cells[r][c].wall = true;
			cells[r][c].visited = false;
			cells[r][c].on_path = false;
		}
	}
}

void draw_cells(SDL_Renderer *renderer, Cell cells[][COLS])
{
	for (int r = 0; r < ROWS; r++)
	{
		for (int c = 0; c < COLS; c++)
		{
			SDL_Rect rect;
			rect.y = 30 * c;
			rect.x = 30 * r;
			rect.h = 30;
			rect.w = 30;

			if (!cells[r][c].wall)
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
				SDL_RenderFillRect(renderer, &rect);
			}
			else if (cells[r][c].on_path)
			{
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				SDL_RenderDrawRect(renderer, &rect);
			}
		}
	}
}

void toggle_wall(int mouse_x, int mouse_y, Cell cells[][COLS])
{
	int row = mouse_x / CELL_SIZE;
	int col = mouse_y / CELL_SIZE;

	cells[row][col].wall = !cells[row][col].wall;
}


/**
 * start a recursive search for paths from the selected cell
 * it searches till it hits the East 'wall' then stops
 * it does not necessarily find the shortest path
 * 
 */
void find_path(Cell target_cell, Cell cells[][COLS])
{
	// TODO : HERE
}

int main(int argc, char *args[])
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *window = SDL_CreateWindow("Maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 300, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	// Initialised cells
	Cell cells[ROWS][COLS];
	initialised_cell(cells);

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
				int mouse_x = event.motion.x;
				int mouse_y = event.motion.y;

				if (event.button.button == SDL_BUTTON_LEFT)
				{	
					toggle_wall(mouse_x, mouse_y, cells);
				}
				// Missing code
				// Call something here on right click to find path
				//...
				
				
				break;
			}
		}

		// Draw black background
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderClear(renderer);

		// Draw Cells
		draw_cells(renderer, cells);

		// Present Render to screen
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
