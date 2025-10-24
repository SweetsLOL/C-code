#include <stdio.h>
//#include "input_functions.h"
#include <stdlib.h>

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
    // Complete this first
    // Fill In Here
    char name[256];
    char location[256];
} Track;

typedef struct album
{
    char title[256];
    char artist[256];
    Genre genre;
    int number_of_tracks;
    Track *tracks;
} Album;

Track read_track(FILE * fptr){
    Track track;
    // reads in a single track from the given file
    // complete this function
    fscanf(fptr, "%[^\n]\n", track.name);
    fscanf(fptr, "%[^\n]\n", track.location);
    return track;
}

// Returns an array of tracks read from the given file
Track* read_tracks(FILE *fptr, int number_of_tracks){
    Track* tracks = malloc(number_of_tracks * sizeof(Track));

    // Put a while loop here which increments an index to read the tracks
    for(int i = 0; i < number_of_tracks; i++){
        tracks[i] = read_track(fptr);
    }   

    return tracks;
}

// Takes a single track and prints it to the terminal
void print_track (Track track){
    // Print title
    printf("Track title is: %s \n", track.name);
    // Print location
    printf("Track location is: %s \n", track.location);
}

// Takes an array of tracks and prints them to the terminal
void print_tracks (Track *tracks, int number_of_tracks){
    // Use a loop with the number of tracks to print tracks
    for(int i = 0; i < number_of_tracks; i++){
        print_track(tracks[i]);
    }
}

Album read_album(FILE *fptr)
{
    Album new_album;

    fscanf(fptr, "%[^\n]\n", new_album.title);
    fscanf(fptr, "%[^\n]\n", new_album.artist);
    fscanf(fptr, "%d\n", &new_album.genre);
    fscanf(fptr, "%d\n", &new_album.number_of_tracks);
    new_album.tracks = read_tracks(fptr, new_album.number_of_tracks);

    return new_album;
}

// Takes a single album and prints it to the terminal along with all its tracks
void print_album(Album album)
{
    printf("Album information is: \n");
    // print out all the albums fields/attributes
    // Complete the missing code.
    printf("ALbum title %s \n", album.title);
    printf("ALbum artist %s \n", album.artist);

    // Print Actual Genre
    switch (album.genre){
        case POP:
            printf("Genre is Pop\n");
            break;
        case CLASSIC:
            printf("Genre is Classical\n");
            break;
        case JAZZ:
            printf("Genre is Jazz\n");
            break;    
        case ROCK:
            printf("Genre is Rock\n");
            break;
        
    }
    
    // print out the tracks
    print_tracks(album.tracks, album.number_of_tracks);
}

// Open the file and read in the album then print them
void main()
{
    FILE *fptr;

    if ((fptr = fopen("album.txt", "r")) == NULL)
    {
        printf("Unable to open file.");
        return;
    }
    else
    {
        Album newAlbum = read_album(fptr);
        print_album(newAlbum);
        fclose(fptr);
    }
}
