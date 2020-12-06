#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// create type to store 1 byte of data (8 bits)
// uint9_t defined in stdint.h (unsigned int)
typedef uint8_t BYTE;

bool jpegcheck(BYTE block[512]);

int main(int argc, char *argv[])
{
    // allow only one command line parameter
    if (argc != 2)
    {
        printf("Useage: ./recover image\n");
        return 1;
    }

    // Open file
    // Returns pointer f to file
    FILE *f = fopen(argv[1], "r");

    // If file unreadable, stop.
    if (f == NULL)
    {
        printf("Cannot open file\n");
        return 1;
    }

    //repeat until end of card
    //read first 512 bytes into a buffer
    int jpegnum = 0;
    char *filename = malloc(7 * sizeof(char)); //TODO free this memory
    bool jpegfound = false;
    while (!feof(f))
    {
        BYTE block[512];
        int bytesinblock = fread(block, 1, 512, f);
        if (jpegcheck(block))
        {
            if (jpegfound == false)
            {
                jpegfound = true;
                sprintf(filename, "%03i.jpg", jpegnum);
                FILE *img = fopen(filename, "w");
                fwrite(block, 1, bytesinblock, img);
                jpegnum++;
                fclose(img);
            }

            else
            {
                sprintf(filename, "%03i.jpg", jpegnum);
                FILE *img = fopen(filename, "w");
                fwrite(block, 1, bytesinblock, img);
                fclose(img);
                jpegnum++;
            }
        }

        else if (jpegfound)
        {
            FILE *img = fopen(filename, "a");
            fwrite(block, 1, bytesinblock, img);
            fclose(img);
        }
    }
    free(filename);
}

// Check if block is beginning of JPEG
bool jpegcheck(BYTE block[])
{
    if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && (block[3] & 0xf0) == 0xe0)
    {
        return true;
    }
    else
    {
        return false;
    }
}