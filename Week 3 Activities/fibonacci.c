#include <stdio.h>

int main(){
    int x = 0;
    int y = 1;
    int z;
    int term;
    int i;
    int next;

    printf("How many terms of the Fibonacci series would you like?");
    scanf("%d", &term);

    for ( i = 1; i <= term; i++)
    {   
        z = x;
        printf("%d\n", z);
        next = x + y;
        x = y;
        y = next;
 
    }
    
}