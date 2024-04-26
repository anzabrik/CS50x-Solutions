// Check that a password has at least one lowercase letter, uppercase letter, number and symbol
// Practice iterating through a string
// Practice using the ctype library

#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

bool valid(string password);

int main(void)
{
    string password = get_string("Enter your password: ");
    if (valid(password))
    {
        printf("Your password is valid!\n");
    }
    else
    {
        printf("Your password needs at least one uppercase letter, lowercase letter, number and symbol\n");
    }
}

// TODO: Complete the Boolean function below
bool valid(string password)
{
    bool lower = false;
    bool upper = false;
    bool number = false;
    bool symbol = false;

    for (int i = 0, N = strlen(password); i < N; i++)
    {
    // Check for upper
        if (isupper(password[i]))
        {
            upper = true;
        }

    // for lower
        else if (islower(password[i]))
        {
            lower = true;
        }

    // for number
        else if (isdigit(password[i]))
        {
            number = true;
        }

    // for symbol
        else if (!isalnum(password[i]))
        {
            symbol = true;
        }
    }
    if (upper == true && lower == true && number == true && symbol == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}
