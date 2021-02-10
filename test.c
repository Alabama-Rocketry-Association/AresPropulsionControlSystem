#include "gpio.h"

int *ask()
{
    int *toRead;
    scanf("%d", toRead);
    int *darray = (int*)malloc(*toRead * sizeof(int));
    for(int i = 0; i < *toRead; i++)
    {
        scanf("%d", darray);
    }
    return darray;
}

void main(){
    int *pins = ask();
    gpio_setup(pins);
}