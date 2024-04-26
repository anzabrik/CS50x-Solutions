#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define ALPHABET 26

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("usage: ./substitution key");
        return 1;
    }

    // Check whether the key is valid
    char *key = argv[1];
    int n = strlen(key);
    if (n != ALPHABET)
    {
        printf("Wrong key");
        return 1;
    }
    for (int i = 0; i < n; i++)
    {
        if (!isalpha(key[i])) // if (0), which means "if non-alpha"
        {
            printf("Wrong key");
            return 1;
        }
    }

    char *plain = get_string("plaintext: ");
    printf("ciphertext: ");
    for (int i = 0, b = strlen(plain); i < b; i++)
    {
        if (isalpha(plain[i]))
        {
            if (islower(plain[i]))
            {
                printf("%c", tolower(key[plain[i] - 'a']));
            }
            else
            {
                printf("%c", toupper(key[plain[i] - 'A']));
            }
        }
        else
        {
            printf("%c", plain[i]);
        }

    }
    printf("\n");





}
 /*
 */