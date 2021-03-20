#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Prompt user for their name and store it in a variable
    string name = get_string("What is your name?\n");
    // Print "hello" followed by the name of the user
    printf("hello, %s\n", name);
}