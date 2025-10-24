#include <stdio.h>
// #include "input_functions.h"
#include <stdlib.h>

// Look previous task on Music Records for an example of how to create the following:
typedef struct track
{
    // Complete this first
    // Fill In Here
    char name[256];
    char location[256];
} Track;

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

int read_number_of_tracks(FILE *fptr){
    int track_num;
    fscanf(fptr, "%d\n", &track_num);
    return track_num;
}


// Open the file and read in the tracks then print them
void main()
{
    FILE *fptr;

    if ((fptr = fopen("input.txt", "r")) == NULL)
    {
        printf("Unable to open file.");
        return;
    }else{
        int number_of_tracks = read_number_of_tracks(fptr);
        Track* tracks = read_tracks(fptr, number_of_tracks);
        print_tracks(tracks, number_of_tracks);
        free(tracks);
        fclose(fptr);
    }
}