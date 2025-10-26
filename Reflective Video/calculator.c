#include <stdio.h>


void addition(int* result, int num1, int num2){
    int sum = num1 + num2;
    *result = sum;
}

void subtraction(int* result, int num1, int num2){
    if (num1>num2){
        int diff = num1 - num2;
        *result = diff;
    } else {
        int diff = num2 - num1;
        *result = diff;
    }
}

void multiplication(int* result, int num1, int num2){
    int product = num1 * num2;
    *result = product;
}

void division(int* result, int num1, int num2){
    int quotient = num1/num2;
    *result = quotient;
}

void main(){
    int choice;
    int num1;
    int num2;
    int result;

    printf("Input the number according to the mathematical operation you want.\n");
    printf("1: Addition\n"
        "2: Subtraction\n"
        "3: Multiplication\n"
        "4: Division\n");
    scanf("%d", &choice);

    printf("First number: ");
    scanf("%d", &num1);

    printf("Second number: ");
    scanf("%d", &num2);
    
    switch(choice){
        case 1:
            addition(&result, num1, num2);
            break;
        case 2:
            subtraction(&result, num1, num2);
            break;
        case 3:
            multiplication(&result, num1, num2);
            break;
        case 4:
            division(&result, num1, num2);
            break;
    }
        
    printf("The result is %d\n", result);

}