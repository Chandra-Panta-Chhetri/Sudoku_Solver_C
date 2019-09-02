#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define SIZE 9
#define EMPTY 0
#define NUM_NONEMPTY_CELLS 25


void print_sudoku(int board[SIZE][SIZE])
{
	printf("********************\n");
	for(int row = 0; row < SIZE; row++)
	{
		for(int col = 0; col < SIZE; col++)
		{
			printf("%d", board[row][col]);
			/* Used to show the vertical seperation between 3 x 3 boxes */
			if((col + 1) % 3 == 0)
			{
				printf(" ** ");
			}
		}
		printf("\n");
		/* Used to show the horizontal seperation between 3 x 3 boxes */
		if((row + 1) % 3 == 0 && (row + 1) != 9)
		{
			printf("********************\n");
		}
	}
	printf("********************\n");
}


/* Checks whether inserting a number, num_to_insert will cause any
	 conflict (duplicate numbers) in a specific row. Returns 0 if row already
	 contains num_to_insert, 1 otherwise. */
int check_row(int board[SIZE][SIZE], int row, int num_to_insert)
{
	for(int i = 0; i < SIZE; i++) {
		if(board[row][i] == num_to_insert) {
			return 0;
		}
	}
	return 1;
}


/* Checks whether inserting a number, num_to_insert will cause any
	 conflict (duplicate numbers) in a specific column. Returns 0 if col already
	 contains num_to_insert, 1 otherwise. */
int check_column(int board[SIZE][SIZE], int col, int num_to_insert)
{
	for(int i = 0; i < SIZE; i++) {
		if(board[i][col] == num_to_insert) {
			return 0;
		}
	}
	return 1;
}


/* Checks whether inserting a number, num_to_insert will cause any
	 conflict (duplicate numbers) in the 3 x 3 box relative to mid_row
	 and mid_col. Returns 0 if 3 x 3 box already contains num_to_insert, 1
	 otherwise. */
int check_around(int board[SIZE][SIZE], int mid_row, int mid_col, int num_to_insert)
{
	/* Ensures to start checking 3 x 3 box for duplicate numbers starting at
	   the top left of the 3 x 3 box */
	int start_row = mid_row - 1;
	int start_col = mid_col - 1;

	for(int i = start_row; i < start_row + 3; i++) {
		for(int j = start_col; j < start_col + 3; j++) {
			if(board[i][j] == num_to_insert) {
				return 0;
			}
		}
	}
	return 1;
}


/* 2 Calls to this function (once with the row and another with the column)
   returns the row or column corresponding to value of index of the
	 cell in the centre of the 3 x 3 box */
int find_midpoint(int index)
{
	if(index >= 0 && index <= 2) {
		return 1;
	}
	else if(index >= 3 && index <= 5) {
		return 4;
	}
	return 7;
}


/* Finds centre cell relative to row and col and checks the 3 x 3 box
   corresponding to index of centre cell for any conflict(duplicate numbers) */
int check_inner_square(int board[SIZE][SIZE], int row, int col, int num_to_insert)
{
	/* contains row of 3 x 3 box relative to row and col */
	int mid_row = find_midpoint(row);
	/* contains column of 3 x 3 box relative to row and col */
	int mid_col = find_midpoint(col);
	return check_around(board, mid_row, mid_col, num_to_insert);
}


/* Checks whether inserting a number, num will cause any conflict
   (duplicate numbers) in the 3 x 3 box, row and col relative to row, and col */
int check_conflict(int board[SIZE][SIZE], int row, int col, int num)
{
	return check_row(board, row, num) && check_column(board, col, num) && check_inner_square(board, row, col, num);
}


/*Finds empty cells and changes the value of row and correspondingly*/
int find_empty_cell(int board[SIZE][SIZE], int *row, int *col)
{
	for(int r = 0; r < SIZE; r++)
	{
		for(int c = 0; c < SIZE; c++)
		{
			if(board[r][c] == EMPTY)
			{
			 *(row) = r;
			 *(col) = c;
			 return 1;
			}
		}
	}
	return 0;
}


/* Solves the given board by using recursive backtracking */
int solve_sudoku(int board[SIZE][SIZE])
{
	int row, col;
	if(!find_empty_cell(board, &row, &col))
	{
		return 1;
	}
	for(int num = 1; num <= SIZE; num++)
	{
		if(check_conflict(board, row, col, num))
		{
			board[row][col] = num;
			/* Checks if inserting num in board[row][col] solves the board.
			   If so, returns 1. Else, recursively solve the board by finding new
				 empty cells and trying numbers 1 - 9(inclusively) until a solution is
				 found, if possible. */
			if(solve_sudoku(board))
			{
				return 1;
			}
			/*if no solution is found by setting board[row][col] = num, set it to
			  empty(0) and try another number (1-9 incusive) */
			board[row][col] = EMPTY;
		}
	}
	/* indicates no solution exist */
	return 0;
}


void Generate_Puzzle(int board[SIZE][SIZE])
{
	/* contains a number(1- 9 inclusive) to be inserted into
	   board[random_row][random_col] provided it is empty(0) */
	int random_num;
	/* indicates the number of non-empty(non-zero) cells in board */
  int num_nonempty = 0;
  int random_row, random_col;
	srand(time(NULL));
	/* Generates a NUM_NONEMPTY_CELLS number of random numbers */
  do
  {
		/*Finds an empty row and column at random, which due to nature of Sudoku
		  must be between 0 and 8 */
    random_row = rand() % 8 + 0;
    random_col = rand() % 8 + 0;

    if(board[random_row][random_col] == EMPTY)
    {
      random_num = rand() % 9 + 1;
			/* Ensures no duplicate numbers will arise once random_num is inserted at
			   random_row and random_col */
      if(check_conflict(board, random_row, random_col, random_num))
      {
        board[random_row][random_col] = random_num;
        num_nonempty++;
      }
    }
  } while(num_nonempty != NUM_NONEMPTY_CELLS);
}


/* Checks the initial board  for any invalid numbers. Returns 0 if invalid,
   1 if valid */
int check_valid_board(int board[SIZE][SIZE], int *num_empty)
{
	for(int row = 0; row < SIZE; row++)
	{
		for(int col = 0; col < SIZE; col++)
		{
			if(board[row][col] < EMPTY || board[row][col] > SIZE)
			{
				return 0;
			}
			if(board[row][col] == EMPTY)
			{
			 *(num_empty) += 1;
			}
		}
	}
	return 1;
}


int main()
{
	int board[SIZE][SIZE]=
	{{0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0}};

	int num_empty = 0;
	int valid = check_valid_board(board, &num_empty);
	/* No point generating random board if not enough empty(0) cells initially */
	if(num_empty >= NUM_NONEMPTY_CELLS) {
		Generate_Puzzle(board);
	}

	printf("Intial Sudoku Puzzle:\n\n");
	print_sudoku(board);
	if(valid == 0)
	{
		printf("\nSorry, the board is not valid (contains duplicate numbers or invalid numbers)\n");
	}
	else if(solve_sudoku(board))
	{
		printf("\n\n\nSolved Sudoku Puzzle:\n\n\n");
		print_sudoku(board);
	}
	else{
		printf("\nSorry, No Solution exists\n");
	}
	return 0;
}
