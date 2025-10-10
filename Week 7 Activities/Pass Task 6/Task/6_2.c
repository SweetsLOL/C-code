#include <stdio.h>
#include <stdlib.h>
#include "input_functions.h"

#define PLAYERS 4

/**
 * This function will prompt user to enter the individual
 * score of each player for all teams
 * (Each team consists of four players).
 * All the input will be stored in the array
 */
void input(int** array, int number_of_teams, int number_of_players){
    // Fill in the blank
    for(int t = 0; t < number_of_teams; t++){
        for(int p = 0; p < number_of_players; p++){
            printf("For team %d, player %d \n", t + 1, p + 1);
            array[t][p] = read_integer("He/She has a score of :");
        }
        }
}

void disp_array(int ** array,int number_of_teams, int number_of_players){
    // Fill in the blank
    for(int t = 0; t < number_of_teams; t++){
    printf("Team %d: ", t);
        for(int p = 0; p < number_of_players; p++){
            printf("%d ", array[t][p]);
        }
        printf("\n");
    }
}

int teamscore(int * array, int number_players){
    // Fill in the blank
    int score = 0;
    for(int i = 0; i < number_players; i++){
        score = score + array[i];
    }
    return score;
}

int** create2DArray(int teams, int players){
    int **temp;
    temp = malloc(teams * sizeof(int *));
    for(int i = 0; i < teams; i++){
        temp[i] = malloc(players * sizeof(int));
    }
    return temp;
}

void free2DArray(int** array, int teams){
    for(int i = 0 ; i < teams; i++){
        free(array[i]);
    }

    free(array);
}

void main(){
    //  prompt user to enter total number of teams
    int number_of_teams = read_integer("Number of teams: \n");

    // Call "create2DArray" array of teams and players
    int ** teams = create2DArray(number_of_teams, PLAYERS);

    // Call "input" to populate team score
    input(teams, number_of_teams, PLAYERS);

    // Call "disp_array" to display team score
    disp_array(teams, number_of_teams, PLAYERS);

    // Print sum of team score here
    for(int t = 0; t < number_of_teams; t++){
        int team_score = teamscore(teams[t], PLAYERS);
        printf("Team %d has a score of %d\n", t, team_score);
    }

    free2DArray(teams, number_of_teams);
}