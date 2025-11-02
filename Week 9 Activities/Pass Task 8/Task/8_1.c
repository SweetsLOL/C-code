#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "input_functions.h"

typedef enum genre
{
    POP = 1,
    CLASSIC = 2,
    JAZZ = 3,
    ROCK = 4
} Genre;

typedef struct track
{
    char name[256];
    char location[256];
} Track;

typedef struct album
{
    char title[256];
    char artist[256];
    char cover[256]; //NEW album cover image
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
    fscanf(fptr, "%[^\n]\n", new_album.cover); //NEW
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
    printf("ALbum cover %s \n", album.cover);

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

int main()
{
    char targetFile[256];
    FILE *fptr;
    int number_of_albums;
    Album *albums;

    bool should_quit = false;

    while (should_quit == false)
    {
        int response = read_integer_in_range("\nOptions:\n1. Read in albums\n2. Display albums\n3. Update album\n4. Exit\n\nYour choice: ", 1, 4);
        switch(response){
            case 1:
                printf("Read albums\n");
                read_string("Please enter the album file name:\n", targetFile);
                if((fptr = fopen(targetFile, "r")) == NULL){
                    printf("Unable to open file.");
                } 
                else 
                {
                    fscanf(fptr, "%d\n", &number_of_albums);
                    albums = malloc(number_of_albums * sizeof(Album));
                    
                    for(int i = 0; i < number_of_albums; i++){
                        albums[i] = read_album(fptr);

                    }
                    printf("Read album successfully");
                }
                break;
            case 2:
                printf("Display albums\n");
                for (int i = 0; i < number_of_albums; i++){
                    printf("Album number %d\n", i+1);
                    print_album(albums[i]);
                    printf("-------------------------\n\n");
                }
                break;
            case 3:
                printf("Update album\n");
                int album_num = -1;
                album_num = read_integer_in_range("Enter the album number: ", 1, number_of_albums);
                read_string("Enter the new album title: ", albums[album_num - 1].title); //index starts with 0, so we -1

                printf("\n\n-------------------\n");
                print_album(albums[album_num - 1]);
                printf("-----------------\n\n");
                break;
            case 4:
                printf("Save and exit\n");
                fclose(fptr); //file was opened in read mode
                if((fptr = fopen(targetFile, "w")) != NULL){
                    fprintf(fptr, "%d\n", number_of_albums); //printf("%d\n", number_of_albums)
                    for(int i = 0; i < number_of_albums; i++){
                        fprintf(fptr, "%s\n", albums[i].title);
                        fprintf(fptr, "%s\n", albums[i].artist);
                        fprintf(fptr, "%s\n", albums[i].cover);
                        fprintf(fptr, "%d\n", albums[i].genre);
                        fprintf(fptr, "%d\n", albums[i].number_of_tracks);

                        for (int j = 0; j < albums[i].number_of_tracks; j++){
                             fprintf(fptr, "%s\n", albums[i].tracks[j].name);
                             fprintf(fptr, "%s\n", albums[i].tracks[j].location);
                        }
                    }
                    fclose(fptr); //closing the file will save it first
                    printf("Any changes made have been saved. \n");

                    //free tracks and albums where memory was allocated via malloc
                    for (int i = 0; i < number_of_albums; i++){
                        free(albums[i].tracks);
                    }
                    free(albums);
                }

                should_quit = true;
                printf("Exiting program.\n");
                break;
        }
    }
    return 0;
}

// Open the file and read in the album then print them
/*int main()
{
    FILE *fptr;

    if ((fptr = fopen("albums.txt", "r")) == NULL)
    {
        printf("Unable to open file.");
        return 1;
    }
    else
    {
        int number_of_albums = 0;
        fscanf(fptr, "%d\n", &number_of_albums);

        for(int i = 0; i < number_of_albums; i++){
            Album newAlbum = read_album(fptr);
            print_album(newAlbum);
        }

        fclose(fptr);
    }

    return 0;
}*/
