#include <stdio.h>
#include <cs50.h>

bool checksum(long num);

// Check if the card number prompted by the user is American Express, Mastercard, Visa or invalid
int main(void)
{
    // Store the card number in a variable
    long card_number = get_long("Number: ");

    // Check if the card number is valid
    if (checksum(card_number))
    {
        // Identify its type based on leading digits and length
        if ((card_number >= 34e13 && card_number < 35e13) || (card_number >= 37e13 && card_number < 38e13))
        {
            printf("AMEX\n");
        }
        else if (card_number >= 51e14 && card_number < 56e14)
        {
            printf("MASTERCARD\n");
        }
        else if ((card_number >= 4e12 && card_number < 5e12) || (card_number >= 4e15 && card_number < 5e15))
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

/**
 * CHECKSUM
 *
 * This function checks that the number provided is a valid card number according to Luhn's algorithm:
 *
 * - Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together.
 * - Add the sum to the sum of the digits that weren’t multiplied by 2.
 * - If the total modulo 10 is congruent to 0, the number is valid
 *
 * The card number's digits are extracted on each iteration using the following principle:
 *
 * - Last digit: card number % 10 = remainder, remainder / 1 = digit
 * - Penultimate digit: (card number - remainder) % 100 = remainder, remainder / 10 = digit
 * - Antepenultimate digit: (card number - remainder) % 1000 = remainder, remainder / 100 = digit
 *
 * After each digit is extracted, depending on its place within the card number (odd or even), it's added to one of the above sums.
 * If the total of the sums added together is a multiple of ten, the function returns bool 'true'.
*/
bool checksum(long num)
{
    // Divisors for card number and remainder
    long remainder;
    long num_divisor = 10;
    long rem_divisor = 1;

    // Index for card number's digits
    int digit;
    int digit_index = 1;

    // Sums for digits at odd (sum1) and even indexes (sum2)
    int sum1 = 0;
    int sum2 = 0;

    // Repeat until there are no more digits left
    do
    {
        // Extract card number's digits per the above principle
        remainder = num % num_divisor;
        digit = remainder / rem_divisor;

        // If digit index is even
        if (digit_index % 2 == 0)
        {
            // Multiply by two
            digit *= 2;

            // If the product is a one-digit number
            if (digit < 10)
            {
                // Add directly to the sum
                sum1 += digit;
            }

            // If the product is a two-digit number
            else
            {
                // Add the product's digits together (i.e. 1 + the remainder of digit / 10)
                sum1 += 1 + (digit % 10);
            }
        }

        // If digit index is odd
        else
        {
            // Add to the sum
            sum2 += digit;
        }

        // Prepare card number for next-digit extraction by subtracting the current digit
        // (i.e. the remainder of the modulus division)
        num -= remainder;

        // Update both card number and remainder divisors to extract the next digit
        num_divisor *= 10;
        rem_divisor *= 10;

        // Update digit index
        digit_index++;

    }
    while (remainder != num);

    // If the total modulo 10 equals 0, return true, otherwise return false
    if ((sum1 + sum2) % 10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }

}