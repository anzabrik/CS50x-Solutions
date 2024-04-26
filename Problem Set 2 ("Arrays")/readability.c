#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Get text & print it out
    string text = get_string("Text: ");
    ///printf("%s\n", text);

    // Count the number of letters
    int letter_count = count_letters(text);
    ///printf("%i\n", letter_count);

    // Count words
    int word_count = count_words(text);
    ///printf("%i\n", word_count);

    // Count sentences
    int sentence_count = count_sentences(text);
    ///printf("%i sentences\n", sentence_count);

    // Count L
    float l = (float) letter_count / word_count * 100;

    // Count S
    float s = (float) sentence_count / word_count * 100;

    // Count grade and round it
    float index = 0.0588 * l - 0.296 * s - 15.8;
    int grade = round(index);

    // Print index out
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

int count_letters(string text)
{
    int sum_letters = 0;
    for (int i = 0, N = strlen(text); i < N; i++)
    {
        if (isalpha(text[i]))
        {
            sum_letters += 1;
        }
    }
    return sum_letters;
}

int count_words(string text)
{
    int sum_words = 0;
    for (int j = 0, N1 = strlen(text); j < N1; j++)
    {
        if (text[j] == ' ')
        {
            sum_words += 1;
        }
    }
    return sum_words + 1;
}

int count_sentences(string text)
{
    int sum_sentences = 0;
    for (int k = 0, N2 = strlen(text); k < N2; k++)
    {
        if (text[k] == '.' || text[k] == '!' || text[k] == '?')
        {
            sum_sentences += 1;
        }
    }
    return sum_sentences;
}
