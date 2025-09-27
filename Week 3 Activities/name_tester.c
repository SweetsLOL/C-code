#include <stdio.h>
#include <string.h>

int main(){
    char name[256];

    printf("Please enter your name:\n");
    scanf("%s", name);

    if(strcmp(name, "Josiah") == 0) { // The curly brackets means code block, if the if condition is met then only will the block of code run. If the two strings are equal, it will give 0
        printf("%s is an awesome name!\n", name);
    } else {
        printf("%s is a ", name);
        int i = 0;
        while(i < 60){
            printf("silly ");
            i = i + 1;
        }
    printf("name!\n");

    }
    

    return 0;
}
