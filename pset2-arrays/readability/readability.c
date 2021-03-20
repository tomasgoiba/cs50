#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


// Run Coleman-Liau readability test on a text fragment
int main(void)
{
    // Store text fragment prompted by the user in 'text'
    string text = get_string("Text: ");

    // Initialize letter, word and sentence counts
    int letters = 0;
    int words = 1;
    int sentences = 0;

    // Iterate over every char in the string
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // If text[i] is an alphabetic character, update letter count
        if (isalpha(text[i]))
        {
            letters++;
        }

        // If text[i] is a whitespace followed by a non-whitespace, update word count
        else if (isblank(text[i]) && !isblank(text[i + 1]))
        {
            words++;
        }

        // If text[i] is '.', '!' or '?', update sentence count
        else if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }

    // Calculate Coleman-Liau index for text fragment
    float letter_average = (float)letters * 100 / (float)words;
    float sentence_average = (float)sentences * 100 / (float)words;
    int index = roundf(letter_average * 0.0588 - sentence_average * 0.296 - 15.8);

    // Print out grade level for text, according to Coleman-Liau formula
    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}