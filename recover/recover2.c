#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    //ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover card.raw");
        return 1;
    }
    //remember filename
    char *infile = argv[1];
    //make file pointer variables to read
    FILE *inptrWhile = fopen(infile, "r");
    if (inptrWhile == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    FILE *inptrInside = fopen(infile, "r");
    if (inptrInside == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    //set the count variable
    int count = 0;
    //set up the while loop
    int *bufferWhile = malloc(512);
    fread(bufferWhile)
    /*
    while (fread(bufferWhile, 1, 512, inptrWhile) == 512)
    {
        int *bufferInside = malloc(512);
        fread(bufferInside,1,512,inptrInside);
        if(bufferInside[0] == 0xff && bufferInside[1] == 0xd8 && bufferInside[2] == 0xff && (bufferInside[3] & 0xf0) == 0xe0)
        {
            char outfile[50];
            sprintf(outfile, "%03i.jpg",count);
            FILE *imgptr = fopen(outfile, "w");
            fwrite(bufferInside,1,512,imgptr);
            fclose(imgptr);
        }
        //fwrite(bufferInside,1,512,imgptr)
        if(bufferInside[0] == 0xff && bufferInside[1] == 0xd8 && bufferInside[2] == 0xff && (bufferInside[3] & 0xf0) == 0xe0)
        {
            printf("found 000.jpg!\n");
            return 0;
        }
        */

    }
    fclose(inptrWhile);
    fclose(inptrInside);
    return 0;

}