/*
Title: Whodunit
Description: Make clue.bmp readable by changing some patterns in the pixels composing it.
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
    if( argc > 3 ) {
        fprintf(stderr, "Usage: ./whodunit infile outfile\n");
        return 1;
   }
    else if(argc < 3) {
        fprintf(stderr, "Usage: ./whodunit infile outfile\n");
        return 1;
    }
    // Define arguments and files.
    string infilep = argv[1];
    string outfilep = argv[2];
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
    // Copy headers
    // Copy the file header into the outfile
    fwrite(&bmpf, sizeof(BITMAPFILEHEADER), 1, outfile);

    // Copy the info header into the outfile
    fwrite(&bmpi, sizeof(BITMAPINFOHEADER), 1, outfile);

    // Get padding of infile
    int padding =  (4 - (bmpi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // The fun part, reading pixels!
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bmpi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bmpi.biWidth; j++)
        {
            // Pixel cache
            RGBTRIPLE triple;

            // Read pixel from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, infile);
            if (triple.rgbtRed == 0xFF) { // Remove those awful red pixels :P
                triple.rgbtRed = 0x00;
            }
            if(triple.rgbtRed == 0x00 || triple.rgbtGreen == 0x00 || triple.rgbtBlue == 0x00) { // Light some pink to the text :D
                triple.rgbtRed = 0xff;
                triple.rgbtGreen = 0x00;
                triple.rgbtBlue = 0xcc;
            }
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