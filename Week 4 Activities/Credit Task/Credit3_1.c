#include <stdio.h>
#include <string.h>

/**
 * This program performs a number of operations and prints
 * results using the same format and operations.
 * Inputs: data entry from the keyboard (see prompts)
 *  Outputs: The user's status and score.
 */

// Reduce the main function down to less than 50 lines of code

void question1(){ //First question
    printf("1. What does the == operator do? fuvk\n");
    printf("(a) calculates an arithmetic solution.\n");
    printf("(b) assigns a value to a variable.\n");
    printf("(c) checks for equality\n(d) draws the '=' character\n");
    printf("Your answer: \n");
}

void question2(){ //Second question
    printf("2. Which is NOT a C keyword?\n");
    printf("(a) when\n");
    printf("(b) const\n");
    printf("(c) unsigned\n(d) do\n");
    printf("Your answer: \n");
}

void question3(){  //Third question
    printf("3. In function call, the actual parameters are separated by\n");
    printf("(a) semicolons\n");
    printf("(b) colons\n");
    printf("(c) commas\n(d) space\n");
    printf("Your answer: \n");
}

void question4(){ //Fourth question
    printf("4. What is the keyword for a global variable?\n");
    printf("(a) global\n");
    printf("(b) namespace\n");
    printf("(c) external\n(d) there is no keyword. to be global the variable must be declared outside of main()\n");
    printf("Your answer: \n");
}

void question5(){
    printf("5. A function prototype always contains:\n");
    printf("(a) a void type\n");
    printf("(b) an integer parameter\n");
    printf("(c) a function name\n(d) a comma\n");
    printf("Your answer: \n");
}

void answer1(char *answer, int *score_upd){
    do
    {
        scanf("%s", answer);
    } while (strcmp(answer, "a") != 0 && strcmp(answer, "b") != 0 && strcmp(answer, "c") != 0 && strcmp(answer, "d") != 0);

    if (strcmp(answer, "c") == 0)
    {
        *score_upd += 1;
        printf("Correct \tYour score is now %d\n", *score_upd);
    }
    else
    {
        printf("Wrong \tYour score is still %d\n", *score_upd);
    }
}

void answer2(char *answer, int *score_upd){
    do
    {
        scanf("%s", answer);
     } while (strcmp(answer, "a") != 0 && strcmp(answer, "b") != 0 && strcmp(answer, "c") != 0 && strcmp(answer, "d") != 0);

    if (strcmp(answer, "a") == 0)
    {
        *score_upd += 1;
        printf("Correct \tYour score is now %d\n", *score_upd);
    }
    else
    {
        printf("Wrong \tYour score is still %d\n", *score_upd);
    }
}

void answer3(char *answer, int *score_upd){
    do
        {
            scanf("%s", answer);
        } while (strcmp(answer, "a") != 0 && strcmp(answer, "b") != 0 && strcmp(answer, "c") != 0 && strcmp(answer, "d") != 0);

        if (strcmp(answer, "c") == 0)
        {
            *score_upd += 1;
            printf("Correct \tYour score is now %d\n", *score_upd);
        }
        else
        {
            printf("Wrong \tYour score is still %d\n", *score_upd);
        }
}

void answer4(char *answer, int *score_upd){
    do
        {
            scanf("%s", answer);
        } while (strcmp(answer, "a") != 0 && strcmp(answer, "b") != 0 && strcmp(answer, "c") != 0 && strcmp(answer, "d") != 0);

        if (strcmp(answer, "d") == 0)
        {
            *score_upd += 1;
            printf("Correct \tYour score is now %d\n", *score_upd);
        }
        else
        {
            printf("Wrong \tYour score is still %d\n", *score_upd);
        }
}

void answer5(char *answer, int *score_upd){
    do
    {
        scanf("%s", answer);
    } while (strcmp(answer, "a") != 0 && strcmp(answer, "b") != 0 && strcmp(answer, "c") != 0 && strcmp(answer, "d") != 0);

    if (strcmp(answer, "c") == 0)
    {
        *score_upd += 1;
        printf("Correct \tYour score is now %d\n", *score_upd);
    }
    else
    {
        printf("Wrong \tYour score is still %d\n", *score_upd);
    }
}


void main()
{
    char answer[8];
    int score = 0;

    printf("Welcome to the C brain scan\n");
    printf("I will ask you some questions...\n");
    printf("Select the best answer and type in the corresponding letter.\n");

    question1();
    answer1(answer, &score);

    question2();
    answer2(answer, &score);

    question3();
    answer3(answer, &score);

    question4();
    answer4(answer, &score);

    question5();
    answer5(answer, &score);

}