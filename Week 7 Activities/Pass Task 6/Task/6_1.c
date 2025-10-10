#include <stdio.h>
#include "input_functions.h"
#include <limits.h>

void display(int *arr){
    printf("Array values are: \n");
    for(int i = 0; i < 10; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(){
    //a. array of size 10
    //int array[10] = {5, 4, 3, 2, 1, -1, -2, -3, -4, -5}; 

    int array[10];
    int start_value = 5;
    for(int i = 0; i < 10; i++){
        array[i] = start_value;
        start_value--; //decrease by 1 
        if(start_value == 0)
            start_value--; //minus value again if 0, to make it -1
    }   
    //b.
    display(array);
    //c.
    int index = read_integer("Index of array: ");
    int newValue = read_integer("New value is: ");

    array[index] = newValue; // like assigning a normal variable, x = 10;

    display(array); //d.
    //e.
    int positiveCount = 0;
    //int max = INT_MIN:
    int max = array[0];

    for(int i = 0; i < 10; i++){
        if(array[i] > 0)
            positiveCount++;
        if(array[i] > max)
            max = array[i];

    }
    printf("Positive count is %d, max is %d\n", positiveCount, max);

    return 0;
}