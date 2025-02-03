// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;
float factor;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    uint8_t header[HEADER_SIZE];
    fread(header, 1, HEADER_SIZE, input);
    fwrite(header, 1, HEADER_SIZE, output);

    // TODO: Read samples from input file and write updated data to output file
    c - HEADER_SIZE) / 2;
    fseek(input, HEADER_SIZE, SEEK_SET);
    int16_t buffer[samples_lenght];
    fread(buffer, 2, samples_lenght, input);
    for (int i = 0; i < samples_lenght; i++)
    {
        buffer[i] *= factor;
    }
    fseek(output, HEADER_SIZE, SEEK_SET);
    fwrite(buffer, 2, samples_lenght, output);

    // Close files
    fclose(input);
    fclose(output);
}
