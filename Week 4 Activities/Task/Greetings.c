#include <stdio.h>
#include <stdbool.h>
#include <string.h>



void greetings(int duration){
    printf("\nDear valued customer:\n");
    if(duration == 0 || duration == 1){
        printf("We would like to share our genuine appreciation for having you as our exclusive club member.\n");
    }else{
        printf("We truely value your continuing support.\n");
    }
    printf("********************************************************************************************\n");

    //unique messages
    if(duration == 0){
        printf("Congratulation for completing your first month journey as our club member!\n");
        printf("To help you explore other services and facilities provides by our club...\n");
        printf("We are offering you 30%% one off discount during your next visit to our club!\n");
    }

    if(duration == 1){
        printf("Congratulation for your first anniversary as our club member!\n");
        printf("value our relationship and look forward to the opportunity to continue serving you.\n");
        printf("We are offering you 20%% one off discount during your next visit to our club!\n");
    }

    if(duration != 0 && duration != 1){
        printf("We look forward to continue serving you for many more wonder years to comes.\n");
        printf("To celeberate your anniversary as our loyal club member...\n");
        printf("We are offering you 10%% one off discount during your next visit to our club!\n");
    }
    printf("Please feel free to call our customer care line : 1-800-1234567 for more information.\n");
    printf("Thank you for being a wonderful and supportive customer, we look forward to serving you a again soon.\n\n");
}
    
void main()
{
    bool repeat = true;
    do
    {
        int duration;
        printf("\nMembership Duration (0.First Month, 1.First Year, 2.Two Years or more):\n");
        scanf("%d", &duration);

        greetings(duration);


        printf("Continue with next message?\n");
        char answer[32];
        scanf("%s", answer);

        repeat = strcmp(answer, "yes") == 0 || strcmp(answer, "Yes") == 0 || strcmp(answer, "YES") == 0;

    } while (repeat);
}