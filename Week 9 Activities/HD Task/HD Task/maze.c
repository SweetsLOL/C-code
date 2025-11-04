#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>

#define CELL_SIZE 30
#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT 300
#define ROWS WINDOW_HEIGHT / CELL_SIZE
#define COLS WINDOW_WIDTH / CELL_SIZE

typedef struct cell
{
    char id[3];

    struct cell *north;
    struct cell *south;
    struct cell *east;
    struct cell *west;

    bool wall;
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

            cells[r][c].wall = true;
            cells[r][c].visited = false;
            cells[r][c].on_path = false;

            // Set neighbour pointers
            cells[r][c].north = (r > 0) ? &cells[r - 1][c] : NULL;
            cells[r][c].south = (r < ROWS - 1) ? &cells[r + 1][c] : NULL;
            cells[r][c].west = (c > 0) ? &cells[r][c - 1] : NULL;
            cells[r][c].east = (c < COLS - 1) ? &cells[r][c + 1] : NULL;
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
            rect.x = c * CELL_SIZE;
            rect.y = r * CELL_SIZE;
            rect.w = CELL_SIZE;
            rect.h = CELL_SIZE;

            if (cells[r][c].on_path)
            {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red for path found
                SDL_RenderFillRect(renderer, &rect);
            }
            else if (!cells[r][c].wall)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // yellow for walkable
                SDL_RenderFillRect(renderer, &rect);
            }

        }
    }
}


void toggle_wall(int mouse_x, int mouse_y, Cell cells[][COLS])
{
    int col = mouse_x / CELL_SIZE;
    int row = mouse_y / CELL_SIZE;

    if (row >= 0 && row < ROWS && col >= 0 && col < COLS)
    {
        cells[row][col].wall = !cells[row][col].wall;
    }
}


bool find_path(Cell *cell)
{
    if (cell == NULL || cell->wall || cell->visited)
        return false;

    cell->visited = true;

    // Base case: reached the rightmost side
    if (cell->east == NULL)
    {
        cell->on_path = true;
        return true;
    }

    // Explore neighbours recursively (east, south, north, west)
    if (find_path(cell->east) || find_path(cell->south) || find_path(cell->north) || find_path(cell->west))
    {
        cell->on_path = true;
        return true;
    }

    return false;
}


void reset_search(Cell cells[][COLS])
{
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            cells[r][c].visited = false;
            cells[r][c].on_path = false;
        }
    }
}


int main(int argc, char *args[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Maze",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    Cell cells[ROWS][COLS];
    initialised_cell(cells);

    bool isRunning = true;
    SDL_Event event;

    while (isRunning)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                isRunning = false;
                break;

            case SDL_MOUSEBUTTONDOWN:
            {
                int mouse_x = event.motion.x;
                int mouse_y = event.motion.y;
                int col = mouse_x / CELL_SIZE;
                int row = mouse_y / CELL_SIZE;

                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    toggle_wall(mouse_x, mouse_y, cells);
                }
                else if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    // Only trigger if right-clicked on a yellow cell in first column
                    if (!cells[row][col].wall && col == 0)
                    {
                        reset_search(cells);
                        find_path(&cells[row][col]);
                    }
                }
                break;
            }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // green background
        SDL_RenderClear(renderer);

        draw_cells(renderer, cells);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
