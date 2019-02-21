#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    //defining data type
    typedef uint8_t  BYTE;
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
    //set the count variable
    int count = 0;
    FILE *imgptr = NULL;
    //set up the while loop
    BYTE buffer[512];
    //setup boolean variable so i can check between 000.jpg vs other
    int check0 = 0; //false = not found the very first beginning yet
    while(fread(buffer, 1, 512, inptr) == 512)
    {

        //FIRST figure out 000.jpg
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {


            //have we already found a beginning?
            if(check0 == 0)
            {
                check0 = 1;
            }
            else
            {
                fclose(imgptr);
            }
            char outfile[50];
            sprintf(outfile,"%03i.jpg", count);
            imgptr = fopen(outfile, "w");
            count++;



        }
        if(check0 == 1)
        {
            fwrite(buffer, 1, 512, imgptr);
        }
    }
    return 0;

}