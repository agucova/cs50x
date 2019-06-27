/*
Title: Recover
Description: Attempt to recover JPEG files from a RAW file scanning for magic bytes.
Author: Agustín Covarrubias
WARNING: This source may have a lot of unnecesary comments.
*/

#include <stdio.h>
#include <cs50.h>
int start = 0; // JPEG file status
int njpg = 0; // Number of jpeg's thus far
FILE* infile; // Init infile
FILE* newfile; // Init outfile
char filename[9];
void writeJPEG();

int main(int argc, char *argv[]) {
    // Check arguments for validity.
    if(!(argc == 2)) {
        fprintf(stderr, "Usage: ./recover infile.raw\n");
        return 1;
   }
    string infilep = argv[1]; // Get path from argument.

    infile = fopen(infilep, "r"); // Open file in read mode.

    // Check if file was opened.
    if (infile == NULL) {
        fprintf(stderr, "Cannot open %s\n", infilep);
        return 2;
    }

    // Get file size.
    fseek(infile, 0L, SEEK_END);
    int size = ftell(infile);
    rewind(infile);

    int nblocks = size / 512; // Number of blocks to scan.
    printf("nblocks: %i\n", nblocks);
    // Iterate blocks
    unsigned char magic[4];
    for(int i = 0; i < nblocks; i++) { // For each block
        for(int j = 0; j < 4; j++) { // For each byte (4 first)
            fread(&magic[j], sizeof(unsigned char), 1, infile); // Read header byte of the block.
        }

        fseek(infile, -4, SEEK_CUR); // Go back to the header

        if (magic[0] == 0xff &&
            magic[1] == 0xd8 &&
            magic[2] == 0xff &&
            (magic[3] & 0xf0) == 0xe0) { // Check for magic signature
                writeJPEG(); // Start writing the JPEG
            }

        else {
            if (start == 1) { // Check if we already have a JPEG open.
                    fwrite(&infile, 1, 512, newfile);  // Continue writing the same JPEG.
                }
            else {
                    fseek(infile, 512, SEEK_CUR);
                }
            }
    }
}
void writeJPEG() {
    if (start == 1) {
        fclose(newfile);
    }
    sprintf(filename, "%03i.jpg", njpg); // Save curr. filename
    newfile = fopen(filename, "w"); // Open new file for reading
    fwrite(&infile, 1, 512, newfile); // Start writing the first block
    njpg++; // Count the new jpeg
}