#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, string argv[])
{
    // Check if there's one and only one command line argument
    if (argc != 2)
    {
        printf("Usage: ./caesar key");
        return 1;
    }

    // Check if each characters in argv[1] is a digit
    for (int i = 0, N = strlen(argv[1]); i < N; i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("Usage: the key must be numerical\n");
            return 1;
        }
    }

    // Convert argv[1] from a string to an int
    int key = atoi(argv[1]);

    // Get plaintext from the user
    string text = get_string("plaintext:  ");

    // Loop through characters in plaintext:
    for (int j = 0, N1 = strlen(text); j < N1; j++)
    {
        // Character+key if it's a letter
        if (islower(text[j]))
        {
            text[j] = (text[j] - 'a' + key) % 26 + 97;
        }
        else if (isupper(text[j]))
        {
            text[j] = (text[j] - 'A' + key) % 26 + 65;
        }
    }
    printf("ciphertext: ");
    printf("%s\n", text);
}

