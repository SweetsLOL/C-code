#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "input_functions.h"

char *check(int actual, int guess){
    if(guess<actual)
    {
        return "Smaller";
    }else if (guess>actual)
    {
        return "Larger";
    }else 
    {
        return "Equal";
    }
    
}

int main(){
    do{
        srand(time(NULL)); //seed with epoch time
        int actual = rand() % 101; //this goes from 0 to 100 because it starts from 0 and will minus 1 from the number we give it
        //rand() % 6 + 5; //5, 6, 7, 8, 9, 10, the number after + is the starting number
        printf("Cheat: %d\n", actual);
        bool is_correct = false;

        do{
            int guess = read_integer("Guess a number?\n"); 

            char *result = check(actual, guess);

            if(strcmp(result, "Smaller") == 0){
                printf("Pick a bigger number\n");
            }
            else if(strcmp(result, "Larger") == 0){
                printf("Pick a smaller number\n");
            }
            else {
                printf("You guessed correctly!\n");
                is_correct = true;
            }
        }while(is_correct == false);
    }while(read_boolean("New game?\n"));
    return 0;
}

    