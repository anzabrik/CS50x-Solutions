#include <cs50.h>
#include <stdio.h>
#include <string.h>

const int BITS_IN_BYTE = 8;

void print_bulb(int bit);
//int arr[8];

int main(void)
{
    // Get message
    string text = get_string("Message: ");
    int arr2[strlen(text)][8];
    // Turn text into decimals, then decimals into binary
    for (int j = 0, N = strlen(text); j < N; j++)
    {
        int x = text[j];

        for (int i = BITS_IN_BYTE - 1; i >= 0 ; i--)
        {
            arr2[j][i] = x % 2;
            x = x / 2;
        }

        for (int m = 0; m < BITS_IN_BYTE; m++)
        {
            print_bulb(arr2[j][m]);
        }
        printf("\n");
    }
}

void print_bulb(int bit)
{
    if (bit == 0)
    {
        // Dark emoji
         printf("\U000026AB");
    }
    else if (bit == 1)
    {
        // Light emoji
         printf("\U0001F7E1");
    }
}
