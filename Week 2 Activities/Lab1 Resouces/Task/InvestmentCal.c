#include <stdio.h>
#include <math.h>

void main()
{ 
    float principle;
    int years;
    float rate;
    float interest;

    printf("Hi there! I will help you calculate compound interest.\nPlease enter the principle amount.\n");
    scanf("%f", &principle);

    printf("Please state for how many years.\n");
    scanf("%d", &years);

    printf("Please enter the interest rate in percentage (e.g. 15 for 15%% )\n");
    scanf("%f", &rate);

    interest =  principle * pow((1 + rate/100), years) - principle;

    printf("The compound interest is: %0.2f", interest);
}
