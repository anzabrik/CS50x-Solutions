#include <cs50.h>
#include <stdio.h>

int main()
{
    long number;
    do
    {
        number = get_long("Number: ");
    }
    while (number < 0);

    long number_for_1st_step = number / 10;//prepares the number for further operations

    // Multiply each of the underlined digits by 2:
    int sum_multiplied_digits = 0;
    for (int i = 0; i < 15; i++)
    {
        int multiplied_digits = number_for_1st_step % 10 * 2;
        number_for_1st_step /= 100;
        //add those products’ digits (i.e., not the products themselves) together:
        sum_multiplied_digits += multiplied_digits % 10;
        multiplied_digits /= 10;
        sum_multiplied_digits += multiplied_digits % 10;
    }
    //printf("M %i\n", sum_multiplied_digits);

    // Calculate the sum of the digits that weren't mulitplied by 2, then add it to the previous sum
    long number_for_2nd_step = number;
    int sum_not_multiplied_digits = 0;
    for (int j = 0; j < 16; j++)
    {
        sum_not_multiplied_digits += number_for_2nd_step % 10;
        number_for_2nd_step /= 100;
    }
    int sum_all_digits = sum_multiplied_digits + sum_not_multiplied_digits;
    //printf("NM %i\n", sum_not_multiplied_digits);
    //printf("Allsum %i\n", sum_all_digits);

    // Checks if the last digit is 0 and prints out the type of the card
    if (sum_all_digits % 10 == 0)
    {
        if ((number >= 340000000000000 && number < 350000000000000) || (number >= 370000000000000 && number < 380000000000000))
        {
            printf("AMEX\n");
        }
        else if (number >= 5100000000000000 && number < 5600000000000000)
        {
            printf("MASTERCARD\n");
        }
        else if ((number >= 4000000000000000 && number < 5000000000000000) || (number >= 4000000000000 && number < 5000000000000))
        {
            printf("VISA\n");
        }
        else
        {
        printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}