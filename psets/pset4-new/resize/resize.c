/*
Title: Resize
Description: Resize a bitmap by a determined factor.
Author: Agustín Covarrubias
WARNING: This source may have a lot of unnecesary comments.
*/
#include <stdio.h>
#include <cs50.h>
#include "bmp.h"

int main(int argc, char *argv[])
{
    // Initialize files
    FILE *infile;
    FILE *outfile;
    // Check arguments
    if (!(argc == 4))
    {
        fprintf(stderr, "Usage: ./resize factor infile outfile\n");
        return 1;
    }
    if (*argv[1] < 0 || *argv[1] > 100)
    {
        fprintf(stderr, "Invalid factor, must be between 0 - 100.\n");
        return 1;
    }
    // Define arguments and files.
    float factor = atof(argv[1]);
    string infilep = argv[2];
    string outfilep = argv[3];
    infile = fopen(infilep, "r");
    outfile = fopen(outfilep, "w");
    if (infile == NULL)
    {
        fprintf(stderr, "Cannot open %s\n", infilep);
        return 2;
    }
    if (outfile == NULL)
    {
        fprintf(stderr, "Cannot open %s\n", outfilep);
        return 3;
    }
    printf("Infile: %s, Outfile: %s, Factor: %.0f\n", infilep, outfilep, factor); // Files ready for reading and writing.

    // Define Headers
    BITMAPFILEHEADER bmpf; // Define the infile header
    fread(&bmpf, sizeof(BITMAPFILEHEADER), 1, infile);

    BITMAPINFOHEADER bmpi; // Define the infile info header
    fread(&bmpi, sizeof(BITMAPINFOHEADER), 1, infile);

    // Check if infile is a bmp
    if (bmpf.bfType != 0x4d42 || bmpf.bfOffBits != 54 || bmpi.biSize != 40 || bmpi.biBitCount != 24 || bmpi.biCompression != 0)
    {
        fclose(outfile);
        fclose(infile);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // Save Infile's padding for later
    int ipadding = ((4 - (bmpi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4);

    // Resize headers!
    int iWidth = bmpi.biWidth;
    int inHeight = abs(bmpi.biHeight);
    bmpi.biWidth *= factor;
    bmpi.biHeight *= factor;

    // Save Outfile's padding for later
    int opadding = ((4 - (bmpi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4);

    // Keep resizing headers
    bmpi.biSizeImage = abs(bmpi.biHeight) * ((bmpi.biWidth * sizeof(RGBTRIPLE)) +
                       opadding); // The size of an image is the width multiplied by the height, with padding
    bmpf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmpi.biSizeImage; // Header + Image

    // Copy headers
    // Copy the file header into the outfile
    fwrite(&bmpf, sizeof(BITMAPFILEHEADER), 1, outfile);
    // Copy the info header into the outfile
    fwrite(&bmpi, sizeof(BITMAPINFOHEADER), 1, outfile);

    // The fun part, reading pixels!
    // iterate over infile's scanlines
    int scanlinel = bmpi.biWidth * sizeof(RGBTRIPLE) + opadding;
    for (int i = 0; i < inHeight; i++)
    {
        RGBTRIPLE scanline[iWidth];
        // if j is the number of the pixel in the scanline, what is the number of the pixel in the oscanline?
        // iterate over pixels in scanline
        for (int j = 0; j < iWidth; j++)
        {
            // Pixel buffer
            RGBTRIPLE triple;

            // Read pixel from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, infile);
            // Save pixel to scanline
            scanline[j] = triple;
        }
        // Write the scanlines, factor times.
        for (int j = 0; j < factor; j++)
        {
            for (int h = 0; h < iWidth; h++)
            {
                for (int z = 0; z < factor; z++)
                {
                    fwrite(&scanline[h], sizeof(RGBTRIPLE), 1, outfile);
                }
            }
            // Padding of scanline
            for (int k = 0; k < opadding; k++)
            {
                fputc(0x00, outfile);
            }
        }
        // Skip padding (by ipadding bits from the current position.)
        fseek(infile, ipadding, SEEK_CUR);
    }

    // Close infile
    fclose(infile);

    // Close outfile
    fclose(outfile);

    // YAY!
    return 0;
}