// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: copy infile outfile\n");
        return 1;
    }

    // remember filenames
    int scale = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    if (scale < 1 || scale > 100)
    {
        fprintf(stderr,"Invalid scale.\n");
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

    //Updating Info Header


    int old_padding = (4 -(bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //modifying the new scale
    int biWidth1 = bi.biWidth;
    int biHeight1 = abs(bi.biHeight);

    bi.biWidth = bi.biWidth * scale;
    bi.biHeight = bi.biHeight * scale;

    // determine new padding for scanlines
    int new_padding = (4 -(bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;


    //modifying the new size of image
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + new_padding) * abs(bi.biHeight);

    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0; i < abs(biHeight1); i++)
    {
        //read scanline 'scale' times
        for (int l = 0; l < scale; l++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < biWidth1; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile
                for (int m = 0; m < scale; m++)
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }

            //add new padding
            for (int k = 0; k < new_padding; k++)
                fputc(0x00, outptr);

            // Return to the beginning of a scanline
            if (l < scale - 1)
                fseek(inptr, -biWidth1 * sizeof(RGBTRIPLE), SEEK_CUR);
        }
        // skip over oldpadding, if any
        fseek(inptr, old_padding, SEEK_CUR);

    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
