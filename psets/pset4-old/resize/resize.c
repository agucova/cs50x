/*
Title: Resize
Description: Resize a bitmap by a determined factor.
Author: Agustín Covarrubias
WARNING: This source may have a lot of unnecesary comments.
*/
#include <stdio.h>
#include <cs50.h>
#include "bmp.h"

int main(int argc, char *argv[]) {
    // Initialize files
    FILE* infile;
    FILE* outfile;
    // Check arguments
    if( argc > 4 ) {
        fprintf(stderr, "Usage: ./resize factor infile outfile\n");
        return 1;
   }
    else if(argc < 4) {
        fprintf(stderr, "Usage: ./resize factor infile outfile\n");
        return 1;
    }
    // Define arguments and files.
    float factor = atof(argv[1]);
    string infilep = argv[2];
    string outfilep = argv[3];
    infile = fopen(infilep, "r");
    outfile = fopen(outfilep, "w");
    if(infile == NULL) {
        fprintf(stderr, "Cannot open %s\n", infilep);
        return 2;
    }
    if(outfile == NULL) {
        fprintf(stderr, "Cannot open %s\n", outfilep);
        return 3;
    }
    printf("Infile: %s, Outfile: %s\n", infilep, outfilep); // Files ready for reading and writing.
    // Define Headers
    BITMAPFILEHEADER bmpf; // Define the infile header
    fread(&bmpf, sizeof(BITMAPFILEHEADER), 1, infile);

    BITMAPINFOHEADER bmpi; // Define the infile info header
    fread(&bmpi, sizeof(BITMAPINFOHEADER), 1, infile);

    // Check if infile is a bmp
    if (bmpf.bfType != 0x4d42 || bmpf.bfOffBits != 54 || bmpi.biSize != 40 ||
        bmpi.biBitCount != 24 || bmpi.biCompression != 0)
    {
        fclose(outfile);
        fclose(infile);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // Resize headers!
    bmpi.biWidth *= factor;
    bmpi.biHeight *= factor;
    bmpi.biSizeImage = bmpi.biWidth * bmpi.biHeight; // The size is width multiplied by the height.
    bmpi.biSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmpi.biSizeImage; // Header + Image

    // Copy headers
    // Copy the file header into the outfile
    fwrite(&bmpf, sizeof(BITMAPFILEHEADER), 1, outfile);
    // Copy the info header into the outfile
    fwrite(&bmpi, sizeof(BITMAPINFOHEADER), 1, outfile);

    // Get padding of infile
    int padding =  (4 - (bmpi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // The fun part, reading pixels!
    // iterate over infile's scanlines
    for (int i = 0, biHeight = bmpi.biHeight; i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bmpi.biWidth; j++)
        {
            // Pixel cache
            RGBTRIPLE triple;

            // Read pixel from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, infile);
            // Resize image
            // Write pixel to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outfile);
        }
    }
        // Skip padding
        fseek(infile, padding, SEEK_CUR);

        // Add back padding
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outfile);
        }

    // Close infile
    fclose(infile);

    // Close outfile
    fclose(outfile);

    // YAY!
    return 0;
}