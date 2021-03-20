#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

// Initialize ABCs
const string ABC = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Function protoype
int validate_key(string s);

// Encript message using a substitution key
int main(int argc, string argv[])
{
    // Check for correct program usage
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Check for valid substitution key
    if (!validate_key(argv[1]))
    {
        return 1;
    }

    // Prompt user for plaintext
    string plaintext = get_string("plaintext: ");

    // Initialize ciphertext variable
    int length = strlen(plaintext);
    char ciphertext[length + 1];  // Extra byte for null character

    // Iterate over plaintext
    for (int i = 0; i < length ; i++)
    {
        // If plaintext[i] is an alphabetic char
        if (isalpha(plaintext[i]))
        {
            // Find its index in ABCs,
            // then add substitution key char at that index to ciphertext[i], respecting plaintext case
            for (int j = 0; j < 26; j++)
            {
                if (plaintext[i] == ABC[j])
                {
                    ciphertext[i] = toupper(argv[1][j]);
                }
                else if (plaintext[i] == tolower(ABC[j]))
                {
                    ciphertext[i] = tolower(argv[1][j]);
                }
            }
        }
        // Otherwise, add it to ciphertext as is
        else
        {
            ciphertext[i] = plaintext[i];
        }
    }

    // Add null character at end of ciphertext
    ciphertext[length] = '\0';

    // Print ciphertext
    printf("ciphertext: %s\n", ciphertext);
}

// Determine if substitution key is valid
int validate_key(string s)
{
    // Check that key is exactly 26 characters long
    if (strlen(s) != 26)
    {
        printf("Key must contain 26 characters\n");
        return false;
    }

    // Check that all characters are alphabetic and only used once
    for (int i = 0; i < 26; i++)
    {
        if (!isalpha(s[i]))
        {
            printf("Key must contain only alphabetic characters\n");
            return false;
        }
        for (int j = i + 1; j < 26; j++)
        {
            if (toupper(s[i]) == toupper(s[j]))
            {
                printf("Key must not contain repeated characters\n");
                return false;
            }
        }
    }
    return true;
}