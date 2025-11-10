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
    // Mix_Music *music;
    char name[256];
    char location[256];
    bool is_playing;
} Track;

typedef struct album
{
    // SDL_Texture * cover;
    char title[256];
    char artist[256];
    char cover[256]; 
    Genre genre;
    int number_of_tracks;
    Track *tracks;
} Album;

Track read_track(FILE * fptr){
    Track track;
    fscanf(fptr, "%[^\n]\n", track.name);
    fscanf(fptr, "%[^\n]\n", track.location);
    track.is_playing = false;
    return track;
}


Track* read_tracks(FILE *fptr, int number_of_tracks){
    Track* tracks = malloc(number_of_tracks * sizeof(Track));
    for(int i = 0; i < number_of_tracks; i++){
        tracks[i] = read_track(fptr);
    }   
    return tracks;
}

Album read_album(FILE *fptr)
{
    Album new_album;
    fscanf(fptr, "%[^\n]\n", new_album.title);
    fscanf(fptr, "%[^\n]\n", new_album.artist);
    fscanf(fptr, "%[^\n]\n", new_album.cover); //NEW
    fscanf(fptr, "%d\n", &new_album.genre);
    fscanf(fptr, "%d\n", &new_album.number_of_tracks);
    new_album.tracks = read_tracks(fptr, new_album.number_of_tracks);   

    return new_album;
}

Album * read_albums(FILE *fptr, int number_of_albums)
{
    Album * albums = malloc(number_of_albums * sizeof(Album));

    for (int i = 0; i < number_of_albums; i++){
        albums[i] = read_album(fptr);
    }

    return albums;
}

void draw_albums(SDL_Renderer * renderer, TTF_Font * font, Album* album, int number_of_albums)    
{
    for(int a = 0; a < number_of_albums; a++){
        Album currentAlbum = album[a];
        // cover image
        SDL_Surface* coverSurf = IMG_Load(currentAlbum.cover);
        SDL_Texture* coverText = SDL_CreateTextureFromSurface(renderer, coverSurf);

        SDL_Rect coverRect;
        coverRect.w = 200;
        coverRect.h = 200;
        coverRect.x = 20;
        coverRect.y = a * 250 + 20; // position of album * index num a + margin of 20 px

        SDL_RenderCopy(renderer, coverText, NULL, &coverRect);
        //Free up resources no longer needed
        SDL_DestroyTexture(coverText);
        SDL_FreeSurface(coverSurf);

        for(int t = 0; t < currentAlbum.number_of_tracks; t++){
            Track currentTrack = currentAlbum.tracks[t];
            //button rects
            SDL_Rect btnRect;
            btnRect.w = 230;
            btnRect.h = 30; 
            btnRect.x = 250;
            btnRect.y = a * 250 + t * 35 + 20; // (album index * 250) + (track index * 35) + 20 px of margin

            if(currentTrack.is_playing)
                SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255);
            else
                SDL_SetRenderDrawColor(renderer, 0, 175, 255, 255);

            SDL_RenderFillRect(renderer, &btnRect);
            //button texts 
            SDL_Color textColor = {0,0,0};
            SDL_Surface* textSurf = TTF_RenderText_Solid(font, currentTrack.name, textColor);
            SDL_Texture* textText = SDL_CreateTextureFromSurface(renderer, textSurf);

            int texW, texH;
            SDL_QueryTexture(textText, NULL, NULL, &texW, &texH);
            SDL_Rect nameRect;
            nameRect.w = texW;
            nameRect.h = texH;
            nameRect.x = btnRect.x + 5;
            nameRect.y = btnRect.y;
            SDL_RenderCopy(renderer, textText, NULL, &nameRect);
            SDL_DestroyTexture(textText);
            SDL_FreeSurface(textSurf);

        }
    }
}

void mouse_clicked(int mouseX, int mouseY, Album * albums, int number_of_albums, Mix_Music *music){
    for (int a = 0; a < number_of_albums; a++){
        Album currentAlbum = albums[a];

        for (int t = 0; t < currentAlbum.number_of_tracks; t++){
            Track* trackPtr = &(currentAlbum.tracks[t]);
            int btnW = 230;
            int btnH = 30;
            int btnX = 250;
            int btnY = a * 250 + t * 35 + 20;

            if(mouseX > btnX && mouseX < (btnX + btnW) && mouseY > btnY && mouseY < (btnY + btnH)){
                printf("Album %d Track %d was clicked\n", a, t);

                trackPtr->is_playing = true;

                Mix_HaltMusic();
                if(music != NULL){
                    Mix_FreeMusic(music);
                }
                music = Mix_LoadMUS(trackPtr->location);
                Mix_PlayMusic(music, -1); //0 for play once then stop
            } else {
                trackPtr->is_playing = false;
            }
            
        }
    }
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
    Mix_Music *music = NULL;
 
    // TODO: open albums.txt
    // ...
    int number_of_albums = 0; // Need to retrieve this from albums.txt
    FILE* fptr = fopen("albums.txt", "r");
    fscanf(fptr, "%d\n", &number_of_albums);
    Album * albums = read_albums(fptr, number_of_albums);

    while (!quit)
    {
        // Poll for event
        if(SDL_PollEvent(&event) > 0){
            switch (event.type){
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    int mouse_x = event.motion.x;
                    int mouse_y = event.motion.y;

                    if (event.button.button == SDL_BUTTON_LEFT)
                    {	
                       mouse_clicked(mouse_x, mouse_y, albums, number_of_albums, music);
                    }
				break;
			}
       }

        // Set Background color
        SDL_SetRenderDrawColor(renderer, 0, 75, 200, 255);
        SDL_RenderClear(renderer);

        draw_albums(renderer, font, albums, number_of_albums);

        SDL_RenderPresent(renderer);
    }

    // Remember to clean texture for album / audio resource for track / free allocated memory
    for (int a = 0; a < number_of_albums; a++){
        free(albums[a].tracks);
    }
    free(albums);

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