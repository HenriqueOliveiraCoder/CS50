#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;
#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    char *infile = argv[1];
    FILE *card = fopen(infile, "r");
    if (card == NULL)
    {
        printf("Could not open %s", infile);
        return 1;
    }

    BYTE buffer[BLOCK_SIZE];
    int img_count = 0;
    FILE *jpeg = NULL;
    while (fread(buffer, 1, BLOCK_SIZE, card) == BLOCK_SIZE)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (jpeg != NULL)
            {
                fclose(jpeg);
            }

            char filename[8];
            sprintf(filename, "%03i.jpg", img_count);

            jpeg = fopen(filename, "w");

            img_count++;
        }

        if (jpeg != NULL)
        {
            fwrite(buffer, 1, BLOCK_SIZE, jpeg);
        }
    }

    fclose(jpeg);
    fclose(card);

    return 0;
}
