#include <stdio.h>

void main()
{
    int appetizer_price;
    int main_price;
    int dessert_price;
    int total_price;

    printf("Enter the appetizer price (in cents)\n");
    /* Insert scanf here */
    scanf("%d", &appetizer_price);

    printf("Enter the main price (in cents)\n");
    /* Insert scanf here */
    scanf("%d", &main_price);

    printf("Enter the dessert price (in cents)\n");
    /* Insert scanf here */
    scanf("%d", &dessert_price);

    total_price = appetizer_price + main_price + dessert_price;
    float total_price_f = (float)total_price/100;
    printf("Total price is $%.2f\n", total_price_f);
}
