from cs50 import get_string


def main():
    """
    Check if the card number prompted by the user is a valid
    American Express, MasterCard or Visa.
    """
    # Get card number from the user as a string
    number = get_string("Number: ")

    # Check if the card number is valid
    if (checksum(number)):

        # Identify its type based on the leading digits and length
        if number[0:2] in {"34", "37"} and len(number) == 15:
            print("AMEX")
        elif number[0:2] in {"51", "52", "53", "54", "55"} and len(number) == 16:
            print("MASTERCARD")
        elif number[0] == "4" and len(number) in {13, 16}:
            print("VISA")
        else:
            print("INVALID")

    # Otherwise print out 'INVALID'
    else:
        print("INVALID")


def checksum(number):
    """
    This function checks that the number provided is a valid card number
    according to Luhn's algorithm.
    """

    # Convert str to an int list using a comprehensive list
    digits = [int(n) for n in number]

    # Initialize main sum
    sum = 0

    # Loop over every other digit in the list, starting with the penultimate digit
    for digit in digits[-2::-2]:  # From penultimate to first, two steps at a time

        # Multiply digit by two
        digit *= 2

        # Add sum of product's digits to the main sum
        if digit < 10:
            sum += digit
        else:
            sum += 1 + (digit % 10)

    # Loop over every other digit in the list, starting with the last digit
    for digit in digits[-1::-2]:  # From last to first, two steps at a time

        # Add digit to the main sum
        sum += digit

    # If total sum modulo 10 equals zero, return True, otherwise return False
    if sum % 10 == 0:
        return True
    else:
        return False


main()