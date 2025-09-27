#include <stdio.h>
#include <strings.h>

int main(){
    int year;
    char repeat[256];
    do{

        printf("Please enter a year:\n");
        scanf("%d", &year);

        int remainder4 = year % 4; //Leap year should be divisible by 4 (remainder == 0)
        int remainder100 = year % 100; //and should not be divisible by 100 (e.g. 1700, 1800, 1900) (remainder != 0)
        int remainder400 = year % 400; //or should be divisible by 400 (remainder == 0)

        if(remainder4 == 0 && remainder100 != 0 || remainder400 == 0){
            printf("This is a leap year");
        } else {
            printf("This is not a leap year");
        }
        printf("Repeat for another input? (Type Y for yes, anything else for no)\n");
        scanf("%s", repeat);

    } while(strcasecmp(repeat, "Y") == 0);

    return 0;
}