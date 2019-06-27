/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(void)
{
    bool odd = d % 2; // (1 = uneven, 0 = even);
    int count = 0;
    int block;
    for(int row = 0; row < d; row++) {
        for(int column = 0; column < d; column++)
        {
            count++; // Increase counter for each block
            block = d*d - count;
            if (!odd && (block == 2)) // Try to replicate the swapped 2 and 1 when odd. (first block)
            {
                block = 1;
            }
            else if (!odd && (block == 1)) // (second block)
            {
                block = 2;
            }
            board[row][column] = block; // Define every block
        }
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for(int row = 0; row < d; row++)
    {
        for(int column = 0; column < d; column++)
        {
            if(board[row][column] == 0)
            {
                printf("__ ");
            }
            else
            {
                printf("%2i ", board[row][column]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */
bool move(int tile)
{
    int tileposr = 0;
    int tileposc = 0;
    int emptyposr = 0;
    int emptyposc = 0;
    if (tile < 0 || tile > (d * d - 1))
    {
        printf("Tile over/underflow detected.\n");
        return false;
    }
    for (int row = 0; row < d; row++) // Since this game is not exactly nasa's mission critical program, I'm going for linear. (sorry)
    {
        for (int column = 0; column < d; column++)
        {
            if (board[row][column] == tile) {
                tileposr = row;
                tileposc = column;
            }
        }
    }
    for (int row = 0; row < d; row++) // Linear again!
    {
        for (int column = 0; column < d; column++)
        {
            if (board[row][column] == 0) {
                emptyposr = row;
                emptyposc = column;
            }
        }
    }
    // If the selected tile is near the empty tile, then: (v2, integer overflow prevention)
    // if((board[emptyposr - 1][emptyposc] == tile) | (board[emptyposr + 1][emptyposc] == tile) | (board[emptyposr][emptyposc - 1] == tile) | (emptyposc < d && board[emptyposr][emptyposc + 1] == tile)) {
    if ((emptyposr > 0 && board[emptyposr - 1][emptyposc] == tile) | (emptyposr < d && board[emptyposr + 1][emptyposc] == tile) | (emptyposc > 0 && board[emptyposr][emptyposc - 1] == tile) | (emptyposc < d && board[emptyposr][emptyposc + 1] == tile))
    {
        // Swap!
        board[tileposr][tileposc] = 0;
        board[emptyposr][emptyposc] = tile;
        return true;
    }
    else
    {
        printf("Tile out of reach.\n");
        return false;
    }
}

/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
bool won(void)
{
    int last = board[0][0]; // Initially, last is
    for (int row = 0; row < d; row++) // Iterate over each row
    {
        for (int column = 0; column < d; column++) // now over each column
        {
            if (board[row][column] != 0 && board[row][column] != last + 1 && row > 0 && column > 0) // if the current tile is not the last one +1, then keep playing.
            {
                return false; // Exit the function without winning.
            }
            last = board[row][column]; // Update the variable just in case. (else not really required.)
        }
    }
    return true;
}