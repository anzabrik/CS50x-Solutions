#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"
typedef uint8_t BYTE;

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }
    FILE *in;
    FILE *out;
    // Open input file for reading
    if ((in = fopen(argv[1], "r")) == NULL)
    {
        printf("Couldn't open the file for reading\n");
        return 1;
    }

    // Read header
    WAVHEADER head;
    fread(&head, sizeof(WAVHEADER), 1, in);

    // Use check_format to ensure WAV format
    if (!check_format(head))
    {
        fclose(in);
        printf("Hell");
        return 1;
    }

    // Open output file for writing
    if ((out = fopen(argv[2], "w")) == NULL)
    {
        printf("Couldn't open the file for writing\n");
        return 1;
    }

    // Write header to file
    fwrite(&head, sizeof(WAVHEADER), 1, out);

    // Use get_block_size to calculate size of block
    int bl_size = get_block_size(head);

    // Write reversed audio to file
    BYTE block[bl_size];// On stack, initialize an array, where each element is a BYTE, element_count = "size of block"

    long position_after_header = ftell(in);
    fseek(in, (-1 * bl_size), SEEK_END); // Set the position indicator 1 block_size away from EOF

    do
    {
        fread(block, sizeof(BYTE), bl_size, in);
        fwrite(block, sizeof(BYTE), bl_size, out);
        fseek(in, (-2 * bl_size), SEEK_CUR);// Set the position indicator 2 block_size away from where it used to be
    }
    while (ftell(in) >= position_after_header);

    fclose(in);
    fclose(out);
}

int check_format(WAVHEADER header)
{
    BYTE wave_format_signature[4] = {87, 65, 86, 69};
    for (int i = 0, n = sizeof(wave_format_signature) / sizeof(BYTE); i < n; i++)
    {
        if (header.format[i] != wave_format_signature[i])
        {
            return 0;
        }
    }
    return 1;
}

int get_block_size(WAVHEADER header)
{
    int block_size = header.numChannels * (header.bitsPerSample / 8);
    return block_size;
}