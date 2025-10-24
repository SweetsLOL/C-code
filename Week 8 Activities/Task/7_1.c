#include <stdio.h>
#include "input_functions.h"

typedef enum genre
{
    POP = 1,
    CLASSIC = 2,
    JAZZ = 3,
    ROCK = 4
} Genre;

typedef struct album
{
    char title[256];
    char artist[256];
    Genre genre;
} Album;

/**
 * Reads in and returns a single album
 * complete the missing lines
 */
Album read_album()
{
    // Insert lines here - use read_string to get input from the
    // terminal for title and artist, use read_integer_in_range
    // to get a genre
    // You only the need validation if reading from the terminal
    // (i.e when using a file later, you can assume the data in
    // the file is correct)

    char album_title[256] = "something"; // write your code here
    char album_artist[256] = "someone";  // write your code here
    Genre album_genre = POP; // write your code here

    Album new_album;
    new_album.title = ...;
    new_album.title = ...;
    new_album.genre = ...;

    return ...
}

/**
 * Takes a single album and prints it to the terminal
 * complete the missing lines:
 */
void print_album(Album album)
{
    printf("Album information is: \n");

    // Insert lines here to print title and artist

    // print actual genre name
    printf("Genre is : %s", ...);
}

/**
 * Reads in albums from the terminal and then prints all the
 * albums to the terminal
 */
void main()
{
    Album album = read_album();
    print_album(album);
}