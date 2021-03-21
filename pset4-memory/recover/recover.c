#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define BLOCK_SIZE 512
#define NAME_SIZE 8

// Define a new type representing an 8-bit unsigned integer
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check that the user has entered exactly one CL argument (i.e. input file)
    if (argc != 2)
    {
        printf("Usage: recover <path/to/input>\n");
        return 1;
    }

    // Store name of input file
    char *infile = argv[1];

    // Open input file and ensure the operation was successful
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Couldn't open %s.\n", infile);
        return 1;
    }

    // Initialize buffer (to store blocks of memory), output file and JPEG file count
    BYTE block[BLOCK_SIZE];
    FILE *outptr;
    int file_count = 0;

    // Repeat until no more units of BLOCK_SIZE bytes are left in the input file
    while (fread(block, BLOCK_SIZE, 1, inptr))
    {
        // Check for JPEG header
        if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && (block[3] & 0xf0) == 0xe0)
        {
            // Close the previous output file
            if (file_count != 0)
            {
                fclose(outptr);
            }

            // Generate a new file name
            char outfile[NAME_SIZE];
            sprintf(outfile, "%03i.jpg", file_count);

            // Create a new output file
            outptr = fopen(outfile, "w");
            if (outptr == NULL)
            {
                printf("Couldn't create %s.\n", outfile);
                return 1;
            }

            // Write the current block of memory to the new output file
            fwrite(block, BLOCK_SIZE, 1, outptr);

            // Add one to the JPEG file count
            file_count++;
        }

        // If no JPEG header is found
        else
        {
            // Keep writing to the current output file
            if (file_count != 0)
            {
                fwrite(block, BLOCK_SIZE, 1, outptr);
            }
        }
    }

    // Close both input and output files
    fclose(inptr);
    fclose(outptr);
}
