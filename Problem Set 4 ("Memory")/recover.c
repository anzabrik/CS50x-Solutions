#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check usage
    if (argc != 2)
    {
        printf("Usage: ./recover forensic.raw");
        return 1;
    }

    // Open input file
    FILE *input;
    if ((input = fopen(argv[1], "r")) == NULL)
    {
        printf("Couldn't open file for reading");
        return 1;
    }

    // Create buffer
    BYTE *buffer = malloc(sizeof(BYTE) * 512);
    if (buffer == NULL)
    {
        printf("Couldn't");
        return 2;
    }

    int jpeg_file_counter = 0;
    char *filename = malloc(sizeof(char) * 10); // Allocate space for the name of a file
    FILE *out;
    int check = 0;

    // Iterate through every 512 bytes until there's a jpeg signature, then create the first jpg and transer info there
    while (check == 0)
    {
        fread(buffer, sizeof(BYTE), 512, input);

        // Check if there is a jpeg signature
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Create&open a new file, write buffer there
            sprintf(filename, "%03i.jpg", jpeg_file_counter);
            out = fopen(filename, "w");
            if (out == NULL)
            {
                printf("hell1");
                return 3;
            }
            fwrite(buffer, sizeof(BYTE), 512, out);
            check = 1;
        }
    }

    // If there is more info, read it into buffer

    while ((fread(buffer, sizeof(BYTE), 512, input)) == 512)
    {
        // Check if there is NO jpeg signature
        if (buffer[0] != 0xff || buffer[1] != 0xd8 || buffer[2] != 0xff || (buffer[3] & 0xf0) != 0xe0)
        {
            // Write the buffer to the already opened file
            fwrite(buffer, sizeof(BYTE), 512, out);
        }
        else
        {
            fclose(out);
            jpeg_file_counter += 1;

            // Create a new file
            sprintf(filename, "%03i.jpg", jpeg_file_counter);
            out = fopen(filename, "w");
            if (out == NULL)
            {
                printf("hell");
                return 3;
            }
            fwrite(buffer, sizeof(BYTE), 512, out);
        }
    }
    fclose(out);
    fclose(input);
    free(filename);
    free(buffer);
}

// Check if there is NO jpeg signature: if (buffer[0] != 0xff || buffer[1] != 0xd8 || buffer[2] != 0xff || buffer[3] < 0xe0 || buffer[3] > 0xef)
// Check if there is a jpeg signature: if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
