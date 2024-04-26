// Write a function to replace vowels with numbers
// Get practice with strings
// Get practice with command line
// Get practice with switch

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

string replace (string output);

int main(int argc, string argv[])
{
     // Check if argc = 2
     if (argc != 2)
     {
        printf("Error: provide one word to convert\n");
        return 1;
     }

     // Implement the replace function
     printf("%s\n", replace(argv[1]));

}

string replace (string word_before)
{
    for (int i = 0, N = strlen(word_before); i < N; i++)
    {
        switch(word_before[i])
        {
            case 'a':
                word_before[i] = '6';
                break;
            case 'e':
                word_before[i] = '3';
                break;
            case 'i':
                word_before[i] = '1';
                break;
            case 'o':
                word_before[i] = '0';
                break;
        }
    }
    return word_before;
}

