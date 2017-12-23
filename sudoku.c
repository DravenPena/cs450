/*Author: Draven Pena
 *Date: 11/10
 *Description: This program takes an input file with a 9x9 sudoku board.Using threads This program will go
 *through the board and check its validaty in 3 ways. First by each row, then each col, and finally through 
 *each grid*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//This struct will represent the sudoku board with a 2d int array, 2 int var, row and col, representing
//the starting row and col, and the result of the sudoku board. Is it valid 1 or invalid 0
typedef struct {
    long row;
    long col;
    long grid;
    int result;
    int sudokuBoard[9][9];
    
} sudokuBoard;


pthread_mutex_t m;

/*Checking functions used by threads*/
void* rowCheck( void *);
void* colCheck( void * );
void* gridCheck( void * );

int main(int argc, char *argv[] )
{
    
    void* rowResult, *colResult, *gridResult;
    int num_threads = 9;
    pthread_mutex_init( &m, NULL );
    
    FILE * file;
    file = fopen( argv[1], "r" );
    if( file == NULL )
    {
        printf( "This didnt open %s", argv[1] ); 
        return 0;
    }
    //build the Board representing the suduko board
    int c;
    sudokuBoard *Board = malloc( sizeof(sudokuBoard) );
    Board->result = 1;
    for(int i = 0; i < 9; i++)
        for( int j = 0; j < 9; j++)
        {
            c = fgetc( file );
            if( isspace(c) )
                c = fgetc( file );
            c = c- '0';
            Board->sudokuBoard[i][j] = c;
        }
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            printf( "%d" , Board->sudokuBoard[i][j] );
        }
        printf("\n");
    }
    
    pthread_t *rowThreads;
    pthread_t *colThreads;
    pthread_t *gridThreads;

    rowThreads = malloc( num_threads * sizeof(pthread_t) );
    colThreads = malloc( num_threads * sizeof(pthread_t) );
    gridThreads = malloc( num_threads * sizeof(pthread_t) );
    //Go through all the rows 
    for( int i = 0; i < num_threads; i++)
        pthread_create( &rowThreads[i], NULL, rowCheck, (void *) Board );

    //go through all the cols 
    for( int i = 0; i < num_threads; i++)
        pthread_create( &colThreads[i], NULL, colCheck, (void *) Board );

    //printf( "main1: Board->row = %d Board->col = %d\n", Board->row, Board->col );
    //go through all the grids
    int count = 0;
    for( int i = 0; i < num_threads/3; i++)
    {
        for( int j = 0; j < num_threads/3; j++)
        {
            pthread_create( &gridThreads[count], NULL, gridCheck, (void *) Board );
            count +=1;
        } 
    }

    for( int i = 0; i < num_threads; i++ )
    {
        pthread_join( rowThreads[i], &rowResult ) ;
        pthread_join( colThreads[i], &colResult );
        pthread_join( gridThreads[i], &gridResult );
    }

    if( (int)rowResult == 0 || (int)colResult == 0 || (int)gridResult == 0 )
        printf( "This is a not Valid Sudoku\n" );
    else
        printf( "This is a Valid Sudoku\n" );

    free(rowThreads);
    free(colThreads);
    free(gridThreads);
    close(file);
    pthread_mutex_destroy(&m);
    printf( "End of program\n");


    return 0;
    
}


void* gridCheck( void* B )
{
    pthread_mutex_lock(&m); 
    int gridsum = 0;
    sudokuBoard *Board = (sudokuBoard*) B;
    Board->grid += 1;
    
    for( int r = ((Board->grid-1)/3)*3; r < ((Board->grid-1)/3)*3 + 3; r++)
    {
        for( int c = ((Board->grid-1)%3)*3; c < ((Board->grid-1)%3)*3 + 3; c++)
        {
            gridsum = gridsum + Board->sudokuBoard[r][c];
        } 
    }
    if( gridsum != 45 )
    {
        Board->result = 0;
        switch( Board->grid)
        {   
            case 1:
                printf( "Problem at top left grid: %d\n", Board->grid );
                break;
            case 2:
                printf( "Problem at top mid grid: %d\n", Board->grid );
                break;
            case 3:
                printf( "Problem at top right grid: %d\n", Board->grid );
                break;
            case 4:
                printf( "Problem at mid left grid: %d\n", Board->grid );
                break;
            case 5:
                printf( "Problem at center grid: %d\n", Board->grid );
                break;
            case 6:
                printf( "Problem at mid right grid: %d\n", Board->grid );
                break;
            case 7:
                printf( "Problem at bottom left grid: %d\n", Board->grid );
                break;
            case 8:
                printf( "Problem at bottom mid grid: %d\n", Board->grid );
                break;
            case 9:
                printf( "Problem at bottom right grid: %d\n", Board->grid );
                break;
        }
    }
    pthread_mutex_unlock(&m); 
    return (void *) Board->result;
}

void* rowCheck( void* B )
{
    //printf("Got to Row Checking\n" );
    pthread_mutex_lock(&m);
    int rowsum = 0;
    sudokuBoard *Board = (sudokuBoard*) B;
    Board->row+=1;
    for( int i = 0; i < 9; i++)
        rowsum = rowsum + Board->sudokuBoard[Board->row-1][i];
    if( rowsum != 45 )
    {
        Board->result = 0;
        printf( "Problem at row: %d\n", Board->row );
    }
    pthread_mutex_unlock(&m);
    return (void *) Board->result;
}

void* colCheck( void* B )
{
    //printf("Got to Col Checking\n" );
    pthread_mutex_lock(&m);
    int colsum = 0;
    sudokuBoard *Board = (sudokuBoard*) B;
    Board->col +=1;
    for( int i = 0; i < 9; i++)
        colsum = colsum + Board->sudokuBoard[i][Board->col-1];
    if( colsum != 45 )
    {
        Board->result = 0;
        printf( "Problem at col: %d\n", Board->col );
    }
    pthread_mutex_unlock(&m);
    return (void *) Board->result;
}

