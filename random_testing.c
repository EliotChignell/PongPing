#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 
int main(void)
{
    // This program will create same sequence of
    // random numbers on every program run
    srand(time(NULL));
    int random_number = rand();

    printf("%d", random_number % 15);

    int random_number2 = rand();

    printf("%d", random_number2 % 15);
   
    return 0;
}