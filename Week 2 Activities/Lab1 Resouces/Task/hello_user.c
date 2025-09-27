#include <stdio.h>
#include <time.h>
#include "functions.h"
#define INCHES 0.393701 // This is a global constant

/**
 * Insert the missing code here into the statements below: 
 * scanf
 * - operator
 */
void main()
{
    char name[256];
    char family_name[256];
    int year_born;

    printf("What is your name ?\n");
    /* Insert scanf here */
    scanf("%s", name);
    printf("Your name is %s !\n", name);

    printf("What is your family name ?\n");
    /* Insert scanf here */
    scanf("%s", family_name);
    printf("Your family name is %s !\n", family_name);
    

    printf("What year were you born ? \n");
    /* Insert scanf here */
    scanf("%d", &year_born); //alternatively use %i. & means address of
    
    // Calculate age
    printf("So you are %d years old\n", get_age(year_born));

    // Calculate height in inches
    float height_cm;
    printf("Enter your height in cms (i.e as a float) \n");
    /* Insert scanf here */
    scanf("%f", &height_cm); //%f float, remember & for address of
    printf("Your height in inches is: %0.2f\n", height_cm * INCHES );
    printf("Finished\n");
}
