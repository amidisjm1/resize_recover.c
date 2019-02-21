// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: resize infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];
    int n = atoi(argv[1]);
    //check for the conditions of n
    if(n < 0 || n >100)
    {
        fprintf(stderr, "n should be 0 < n <= 100");
        return 1;
    }

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    //update header and calculate padding
    int oldWidth = bi.biWidth;
    int oldHeight = bi.biHeight;
    int newWidth = oldWidth*n;
    int newHeight = oldHeight*n;
    // determine padding for scanlines
    bi.biHeight = newHeight;
    bi.biWidth = newWidth;
    int oldPadding = (4 - (oldWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int newPadding = (4 - (newWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    bi.biSizeImage = ((sizeof(RGBTRIPLE)*newWidth) + newPadding)*abs(newHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);


    // for each row
    for (int i = 0, biHeight = abs(oldHeight); i < biHeight; i++)
    {
        // for n-1 times for vertically resizing
        for (int j = 0; j < n-1; j++)
        {
            //write pixels
            for (int k = 0; k < oldWidth; k++)
            {
                RGBTRIPLE triple;
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                for (int l = 0; l < n; l++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            //write paddings
            for (int m = 0; m < newPadding; m++)
            {
                fputc(0x00, outptr);
            }
            //send infile cursor back
            fseek(inptr, -(oldWidth*sizeof(RGBTRIPLE)), SEEK_CUR);
        }
        //write remaing pixels and padding to outfile
        for (int kone = 0; kone < oldWidth; kone++)
        {
            RGBTRIPLE triple;
            fread(&triple, sizeof(RGBTRIPLE),1,inptr);
            for (int lone = 0; lone < n; lone++)
            {
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
        }
        for (int mone = 0; mone<newPadding; mone++)
        {
            fputc(0x00, outptr);
        }
        //send incursor to the next row
        fseek(inptr, oldPadding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
