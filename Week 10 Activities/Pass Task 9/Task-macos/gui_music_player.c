#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
//#include "input_functions.h"

typedef enum genre
{
    POP = 1,
    CLASSIC = 2,
    JAZZ = 3,
    ROCK = 4
} Genre;

// Look previous task on Music Records for an example of how to create the following:
typedef struct track
{
    Mix_Music *music;
    // Complete this first
    // Fill In Here
} Track;

typedef struct album
{
    SDL_Texture * cover;
    
    char title[256];
    char artist[256];
    Genre genre;
    int number_of_tracks;
    Track *tracks;
} Album;

Track read_track(FILE *fptr)
{
    Track track;

    // TODO

    return track;
}

// Returns an array of tracks read from the given file
Track *read_tracks(FILE *fptr, int number_of_tracks)
{
    Track *tracks = malloc(number_of_tracks * sizeof(Track));
    
    // TODO 

    return tracks;
}

Album read_albums(FILE *fptr)
{
    Album album;

    // TODO

    return album;
}

Album * read_albums(FILE *fptr, int number_of_albums)
{
    Album * albums = malloc(number_of_albums * sizeof(Album));

    // TODO

    return albums;
}

void draw_albums(Album* album)
{
    // Draw albums
    ...
}

int main(int argc, char ** argv) {
    bool quit = false;
    SDL_Event event;

    // SDL Init
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    Mix_Init(MIX_INIT_MP3);
    TTF_Init();

    // Text Related
	TTF_Font *font = TTF_OpenFont("arial.ttf", 25);
	SDL_Color color = {255, 255, 255};
 
    SDL_Window * window = SDL_CreateWindow("Music Player", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    
    // Open Audio Channel
    Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 );
 
    // TODO: open albums.txt
    ...
    int number_of_albums = 0; // Need to retrieve this from albums.txt

    int mouse_x;
    int mouse_y;

    while (!quit)
    {
        // Poll for event
        if(SDL_PollEvent(&event) > 0){
            switch (event.type){
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    mouse_x = event.motion.x;
                    mouse_y = event.motion.y;

                    if (event.button.button == SDL_BUTTON_LEFT)
                    {	
                        // If click on track, play music
                        // Determine which track user has clicked
                        Mix_Music *music = albums[...].track[...].music;
                        Mix_PlayMusic( music, -1 );
                    }
				break;
			}
       }

        // Set Background color
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);
    }

    // Remember to clean texture for album / audio resource for track / free allocated memory

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
	TTF_Quit();
    
    IMG_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
 
    return 0;
}