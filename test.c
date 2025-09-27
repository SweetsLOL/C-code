#include <stdio.h>

void assign1(int x){
    x = 20;
    printf("x in function after assigning of new value = %d\n", x);
}

void assign2(int *x){
    *x = 30;
    printf("x in function after assigning of new value = %d\n", *x);
}

int main(){
    int x = 1;


    assign1(x);
    printf("The value of x is %d\n", x);
    
    assign2(&x);
    printf("The value of x is %d\n", x);

    return 0;
}