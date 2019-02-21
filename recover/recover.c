#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    typedef uint8_t BYTE;
    //ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover card.raw");
        return 1;
    }
    //remember filename
    char *infile = argv[1];
    //make file pointer variables to read
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    //set the necessary variables
    int count = 0;
    FILE *imgptr = NULL;
    BYTE buffer[512];
    while (fread(buffer, 1, 512, inptr) == 512)
    {
        if(count == 0)
        {
            if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
            {
                char outfile[10];
                sprintf(outfile, "%03i.jpg", count);
                imgptr = fopen(outfile, "w");
                fwrite(buffer, 1, 512, imgptr);
                count++;
            }
        }
        else
        {
            if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
            {
                fclose(imgptr);
                char outfile[10];
                sprintf(outfile, "%03i.jpg", count);
                imgptr = fopen(outfile, "w");
                fwrite(buffer, 1, 512, imgptr);
                count++;
            }
            else
            {
                fwrite(buffer, 1, 512, imgptr);
            }

        }
    }
    fclose(imgptr);
    fclose(inptr);
    return 0;
}