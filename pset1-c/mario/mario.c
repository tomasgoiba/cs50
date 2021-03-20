#include <stdio.h>
#include <cs50.h>

int get_height(string prompt);

// Print out a pyramid with '#' for the height specified by the user
int main(void)
{
    // Store int from user in a variable called 'height'
    int height = get_height("Height: ");

    // Loop over the number of rows
    for (int row = 1; row < height + 1; row++)
    {
        // For each row, first print out the blank spaces on the left
        for (int blank = 0; blank < (height - row); blank++)
        {
            printf(" ");
        }

        // Second, the hashes
        for (int hash = 0; hash < row; hash++)
        {
            printf("#");
        }

        // Third, a "gap" between pyramids (two spaces)
        printf("  ");

        // Finally, the adjacent pyramid
        for (int hash = 0; hash < row; hash++)
        {
            printf("#");
        }

        // Prepare for the next row
        printf("\n");
    }
}

// Propmt user for a positive integer
int get_height(string prompt)
{
    int n;

    // Repeat until user enters an integer between 1 and 8, inclusive
    do
    {
        n = get_int("%s", prompt);
    }
    while (n < 1 || n > 8);
    return n;
}

