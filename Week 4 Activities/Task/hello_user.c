#include <stdio.h>
#include <time.h>
#include "input_functions.h"
#define INCHES 0.393701 // This is a global constant

void main()
{
    char name[256];
    char family_name[256];
    int year_born = 0; //initialize to zero

    //printf("What is your name ?\n");
	//scanf("%s", name); //no & as string is a char array
    read_string("What is your name ?\n", name);
    printf("Your name is %s !\n", name);

    //printf("What is your family name ?\n");
	//scanf("%s", family_name);
    read_string("What is your family name?\n", family_name);
    printf("Your family name is %s !\n", family_name);

    //printf("What year were you born ? \n");
	//scanf("%d", &year_born); //primitive value, not an array, so with & to access the value's addres
    year_born = read_integer("What year were you born? \n");
    // Calculate age
    printf("So you are %d years old\n", get_age_from_year_born(year_born));

    // Calculate height in inches from cm
    float height_cm;
    //printf("Enter your height in cms (i.e as a float) \n");
	//scanf("%f", &height_cm); //primitive value, not an array. use &.
    height_cm = read_float("Enter your height in cms (i.e as a float) \n");
    printf("Your height in inches is: %0.2f\n", height_cm * INCHES );
	
}
