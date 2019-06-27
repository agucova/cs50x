/*
Title: Recover (Clean Attemp n'2)
Description: Attempt to recover JPEG files from a RAW file scanning for magic bytes.
Author: Agustín Covarrubias
WARNING: This source may have a lot of unnecesary comments.
*/

#include <stdio.h>
#include <cs50.h>
// Global Variables
FILE *infile;
FILE *outfile;
unsigned char buffer[512];
void readFile();
void writeJPEG();
int writing = 0;
int njpeg = 0;

int main(int argc, char *argv[])
{
    // Check arguments for validity.
    if (!(argc == 2))
    {
        fprintf(stderr, "Usage: ./recover infile.raw\n");
        return 1;
    }
    string infilep = argv[1]; // Get path from argument.

    infile = fopen(infilep, "r"); // Open file in read mode.

    // Check if file was opened.
    if (infile == NULL)
    {
        fprintf(stderr, "Cannot open %s\n", infilep);
        return 2;
    }
    readFile();
}

void readFile()
{
    while (fread(buffer, 1, 512, infile) == 512)// Loop each block until bytes are missing (EOF)
    {
        if (buffer[0] == 0xff && // If a JPEG header is found
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            if (writing != 0) // Close the file if a new header is found
            {
                fclose(outfile);
            }
            else
            {
                writing = 1; // First jpeg, start writing.
            }
            writeJPEG();
            njpeg++; // New jpeg count.
        }
        else if (writing == 1) // If writing is already on course.
        {
            fwrite(&buffer, sizeof(unsigned char), 512, outfile); // Write following buffer
        }
    }
    fclose(infile);
    fclose(outfile);
}
void writeJPEG()
{
    char filename[8];
    sprintf(filename, "%03i.jpg", njpeg); // Generate a filename
    outfile = fopen(filename, "w"); // Open new file.
    fwrite(&buffer, sizeof(unsigned char), 512, outfile); // Write buffer
}