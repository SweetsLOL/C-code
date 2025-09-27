#include <stdio.h>
#include <strings.h>
int main(){
    int lowest = 38;
    int t; //t is for temperature
    char backup_power[256];
    do{
        printf("What is the temperature reading?\n");
        scanf("%d", &t);

        if(t < lowest)
        {
            lowest = t;
        } else {
            printf("Do you want to turn on the backup power? (Type Y for yes and N for no)");
            scanf("%s", backup_power);
        }
        
        if(t < 50)
        {
            printf("Normal mode of operation.\n");
            continue;
        }

        if(t <= 80 || strcasecmp(backup_power, "Y") == 0){
            printf("Circulating fan turned on.\n");
            continue;
        } else {
            break;
        }
    } while(1);

    printf("Turning off equipment.\n");
    printf("Lowest temperature = %d\n", lowest);
}