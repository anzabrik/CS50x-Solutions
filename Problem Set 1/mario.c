#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height = 0;
    do
    {
        height = get_int("Height: ");
    }
    while (height <= 0 || height > 8);


    for (int i = 0; i < height; i++)
    {
        for (int j = height - 1; j > i; j--)
        {
            printf(" ");
        }
        for (int k = 0; k <= i; k++)
        {
            printf("#");
        }
        printf("  ");
        for (int l = 0; l <= i; l++)
        {
            printf("#");
        }
        /*for (int m = height - 1; m > i; m--)
        {
            printf(" ");
        }*/
        printf("\n");
    }

}