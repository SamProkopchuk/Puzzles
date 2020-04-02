#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void print_sudoku(int sudoku[9][9])
{
  printf("The Sudoku contains:\n");
  for (int j=0; j<9; j++)
  {
    for (int i=0; i<9;i++)
    {
      printf("%d  ",sudoku[j][i]);
    }
    printf("\n");
  }
}

int check_sudoku(int sudoku[9][9])
{
  /*
   *  This function checks whether a Sudoku is properly
   * solved. That means that each row, column, and
   * 3x3 subgrid uses each digit only once.
   * 
   *  If *ignores* zeros, so you can use it to check
   * that a partial solution is valid
   */
  int row_counts;
  int col_counts;
  int sub_counts;
  for (int i=1; i<10; i++) {
    // Check rows and columns
    for (int row=0;row<9;row++) {
      row_counts=0;
      col_counts=0;
      for (int col=0;col<9;col++) {
        if (sudoku[row][col] == i) {
          row_counts++;
        }
        if (sudoku[col][row] == i) {
          col_counts++;
        }
      }
      if ((row_counts > 1) || (col_counts > 1)) {
        return 0;
      }
    }

    // Check all 3x3 subgrids
    for (int sub_row=0; sub_row<3; sub_row++) {
      for (int sub_col=0; sub_col<3; sub_col++) {
        sub_counts=0;
        for (int row_delta=0; row_delta<3; row_delta++) {
          for (int col_delta=0; col_delta<3; col_delta++) {
            if (sudoku[sub_row*3 + row_delta][sub_col*3 + col_delta] == i) {
              sub_counts++;
            }
          }
        }
        if (sub_counts > 1) {
          return 0;
        }
      }
    }
  }
  return 1;
}

int solved(int sudoku[9][9])
{
  /*
   * This function checks whether a given Sudoku is
   * completely solved (it has no zeros, all digits
   * are used once per row, column, and sub-grid.
   * It returns 1 if the Sudoku is solved, zero
   * otherwise
   */
  for (int row=0; row<9; row++) {
    for (int col=0; col<9; col++) {
      if (sudoku[row][col] == 0) {
        return 0;
      }
    }
  }
  return check_sudoku(sudoku);
}

void init_poss(int sudoku[9][9], int pos[9][9][9]) {
  // For every sudoku entry,
  // if it's zero - set everything in pos at that coord to 1
  // Otherwise -    set everything in pos at that coord to 0
  for (int row=0; row<9; row++) {
    for (int col=0; col<9; col++) {
      for (int i=0; i<9; i++) {
        if (sudoku[row][col] == 0) {
          pos[row][col][i] = 1;
        } else {
          pos[row][col][i] = 0;
        }
      }
    }
  }
}

void set_to_zero(int pos[9][9][9], int n, int row, int col, int *chng) {
  // Sets row, col, n to 0
  // returns chng + 1 if row, col, n was 1
  if (pos[row][col][n] == 1) {
      pos[row][col][n] = 0;
      *chng += 1;
  }
}

void clear_common(int pos[9][9][9], int n, int nrow, int ncol, int *chng) {
  // For every number, remove possibility that the same number could
  // be in the same row, column, or cell.
  n-=1;
  for (int rowcol=0; rowcol<9; rowcol++) {
    set_to_zero(pos, n, nrow, rowcol, chng);
    set_to_zero(pos, n, rowcol, ncol, chng);
  }

  // Find coordinate of top right of subgrid n lies in.
  int subgridr = nrow-(nrow % 3);
  int subgridc = ncol-(ncol % 3);

  for (int row_delta=0; row_delta<3; row_delta++) {
    for (int col_delta=0; col_delta<3; col_delta++) {
      set_to_zero(pos, n, subgridr+row_delta, subgridc+col_delta, chng);
    }
  }
}

int clear_impos(int sudoku[9][9], int pos[9][9][9]) {
  // Clears every impossible guess based upon every nonzero entry.
  if (check_sudoku(sudoku) == 0) {
    return 0;
  }
  int chng=0;
  for (int row=0; row<9; row++) {
    for (int col=0; col<9; col++) {
      if (sudoku[row][col] != 0) {
        clear_common(pos, sudoku[row][col], row, col, &chng);
      }
    }
  }
  return (chng != 0);
}

int only_one_num(int pos[9][9][9], int row, int col) {
  // Returns the one number something can be if it can be one number
  // Otherwise returns -1
  int one_count=0;
  int one_index;
  for (int i=0; i<9; i++) {
    if (pos[row][col][i] == 1) {
      one_count++;
      one_index = i;
    }
  }
  if (one_count == 1) {
    return one_index+1;
  }
  return -1;
}

void set_all_single_options(int sudoku[9][9], int pos[9][9][9])
{
  // Sets all entries that can be one number to that number.
  int num;
  for (int row=0; row<9; row++) {
    for (int col=0; col<9; col++) {
      num = only_one_num(pos, row, col);
      if ((sudoku[row][col] == 0) && (num != -1)) {
        sudoku[row][col] = num;
      }
    }
  }
}

int min_nonzero_pos(int pos[9][9][9], int *nrow, int *ncol, int *nn)
{
  // returns 10 if there is all zeros in pos at that index.
  int tempn;
  int min = 10;
  int count;
  for (int row=0; row<9; row++) {
    for (int col=0; col<9; col++) {
      count = 0;
      for (int n=0; n<9; n++) {
        if (pos[row][col][n] == 1) {
          tempn = n;
          count++;
        }
      }
      if ((count < min) && (count > 1)) {
        min = count;
        *nrow = row;
        *ncol = col;
        *nn = tempn;
      }
    }
  }
  return min;
}

void cpy_sudoku(int sudoku_dst[9][9], int sudoku_src[9][9])
{
  for (int row=0; row<9; row++) {
    for (int col=0; col<9; col++) {
      sudoku_dst[row][col] = sudoku_src[row][col];
    }
  }
}

void cpy_pos(int pos_dst[9][9][9], int pos_src[9][9][9])
{
  for (int row=0; row<9; row++) {
    for (int col=0; col<9; col++) {
      for (int n=0; n<9; n++) {
        pos_dst[row][col][n] = pos_src[row][col][n];
      }
    }
  }
}

void zero_all_but(int pos[9][9][9], int nrow, int ncol, int nn)
{
  int chng=0;
  for (int n=0; n<9; n++) {
    if (n != nn) {
      set_to_zero(pos, n, nrow, ncol, &chng);
    }
  }
}

void sift_pos(int sudoku[9][9], int pos[9][9][9], int depth)
{
  // solves sudoku every step by minimizing pos
  // and filling in sudoku based upon minimal pos.
  if (solved(sudoku)) {
    return;
  } if (check_sudoku(sudoku) == 0) {
    return;
  } if (clear_impos(sudoku, pos) == 0) {
    int sudoku_cpy[9][9];
    int pos_cpy[9][9][9];
    cpy_sudoku(sudoku_cpy, sudoku);
    cpy_pos(pos_cpy, pos);
    int nrow, ncol, nn;
    if (min_nonzero_pos(pos, &nrow, &ncol, &nn) == 10) {
      return;
    }
    zero_all_but(pos_cpy, nrow, ncol, nn);
    set_all_single_options(sudoku_cpy, pos_cpy);
    sift_pos(sudoku_cpy, pos_cpy, depth+1);

    if (solved(sudoku_cpy)) {
      cpy_sudoku(sudoku, sudoku_cpy);
      return;
    } else {
      int chng=0;
      set_to_zero(pos, nn, nrow, ncol, &chng);
    }
  }
  set_all_single_options(sudoku, pos);

  sift_pos(sudoku, pos, depth);
}

void solve_sudoku(int sudoku[9][9], int depth)
{
  /*
   * This function solves an input Sudoku, the
   * input has a value of 0 for any entries that
   * are not yet decided. If no solution exists
   * the function returns the input array *exactly
   * as it was* when the function was called.
   */

  int pos[9][9][9];
  init_poss(sudoku, pos);

  int sudoku_cpy[9][9];
  cpy_sudoku(sudoku_cpy, sudoku);

  sift_pos(sudoku, pos, depth);

  if (!solved(sudoku)) {
    print_sudoku(sudoku);
    cpy_sudoku(sudoku, sudoku_cpy);
  }
}

int main()
{
//   int Sudoku[9][9] = {
// { 0,0,0,0,0,0,0,0,0 },
// { 0,0,0,0,0,0,0,0,0 },
// { 0,0,0,0,0,0,0,0,0 },
// { 0,0,0,0,0,0,0,0,0 },
// { 0,0,0,0,0,0,0,0,0 },
// { 0,0,0,0,0,0,0,0,0 },
// { 0,0,0,0,0,0,0,0,0 },
// { 0,0,0,0,0,0,0,0,0 },
// { 0,0,0,0,0,0,0,0,0 }
// };

//   int Sudoku[9][9] = {
// { 1,0,0,0,0,7,0,9,0 },
// { 0,3,0,0,2,0,0,0,8 },
// { 0,0,9,6,0,0,5,0,0 },
// { 0,0,5,3,0,0,9,0,0 },
// { 0,1,0,0,8,0,0,0,2 },
// { 6,0,0,0,0,4,0,0,0 },
// { 3,0,0,0,0,0,0,1,0 },
// { 0,4,1,0,0,0,0,0,7 },
// { 0,0,7,0,0,0,3,0,0 }
// };
   int Sudoku[9][9]={
{6, 0, 0, 0, 0, 8, 9, 4, 0, },
{9, 0, 0, 0, 0, 6, 1, 0, 0, },
{0, 7, 0, 0, 4, 0, 0, 0, 0, },
{2, 0, 0, 6, 1, 0, 0, 0, 0, },
{0, 0, 0, 0, 0, 0, 2, 0, 0, },
{0, 8, 9, 0, 0, 2, 0, 0, 0, },
{0, 0, 0, 0, 6, 0, 0, 0, 5, },
{0, 0, 0, 0, 0, 0, 0, 3, 0, },
{8, 0, 0, 0, 0, 1, 6, 0, 0, }
};

//   int Sudoku[9][9]={
// {1, 0, 0, 0, 0, 0, 0, 0, 0, },
// {0, 0, 0, 1, 0, 0, 0, 0, 0, },
// {0, 0, 0, 0, 0, 0, 1, 0, 0, },
// {0, 1, 0, 0, 0, 0, 0, 0, 0, },
// {0, 0, 0, 0, 1, 0, 0, 0, 0, },
// {0, 0, 0, 0, 0, 0, 0, 1, 0, },
// {0, 0, 1, 0, 0, 0, 0, 0, 0, },
// {0, 0, 0, 0, 0, 1, 0, 0, 0, },
// {0, 0, 0, 0, 0, 0, 0, 0, 2, }
// };

  printf("Input puzzle is:\n");
  print_sudoku(Sudoku);
  
  solve_sudoku(Sudoku,0);
  
  printf("Solution is:\n");
  print_sudoku(Sudoku);
  
}
