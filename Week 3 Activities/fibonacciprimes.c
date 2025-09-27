#include <stdio.h>
#include <math.h>
#include <stdbool.h>

int main(){
    int term;
    int i = 1;
    int x = 0;
    int y = 1;
    int current_number;
    int next;
    

    printf("Please enter the number of terms for the Fibonacci series.\n");
    scanf("%d", &term);

    printf("Among the first %d term(s) of Fibonacci series that are also prime numbers:\n", term);

    
    do{ 
        current_number = x;
        bool isPrime = true;

        if(current_number > 1){
             for(int checkprime = 2; checkprime <= sqrt(current_number); checkprime++){ // cant use int checkprime = z because then it will check if z % z == 0, which will always make isPrime = false for every number
                if(current_number % checkprime == 0){
                    isPrime = false;
                    break;
                }
            }
            if(isPrime){
                printf("Term: %d\n%d\n", i, current_number);
            }
        }
        next = x + y;
        x = y;
        y = next;
        i++;
    }while(i <= term);
    
    return 0;
}   
