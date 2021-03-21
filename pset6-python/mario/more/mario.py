from cs50 import get_int


def main():
    """
    Print out a pyramid with '#' for the height chosen by the user
    """

    # Store int from user in a variable
    height = get_valid_int("Height: ")

    # Loop over the number of rows
    for row in range(1, height + 1):

        # Calculate and store the number of blanks and hashes at 'row'
        blanks = " " * (height - row)
        pyramid = "#" * row

        # Format row with f-string
        print(f"{blanks}{pyramid}  {pyramid}")


# Propmt user for a valid integer
def get_valid_int(prompt):
    while True:
        n = get_int(prompt)

        # Repeat until user inputs an int between 1-8, inclusive
        if n > 0 and n < 9:
            break
    return n


main()

