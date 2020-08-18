#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_board(const int rows, const int cols, int board[rows+2][cols+2]) {
	// Assumes board is zero padded (whic is why board's size is int[rows+2][cols+2])
	for (int row=0; row<rows; row++) {
		printf("|");
		for (int col=0; col<cols; col++) {
			printf("%d|", board[row+1][col+1]);
		}
		printf("\n");
	}
}

int count_nonzero(const int rows, const int cols, int board[rows+2][cols+2]) {
	// Assumes board is zero padded (whic is why board's size is int[rows+2][cols+2])
	int c;
	for (int row=0; row<rows; row++) {
		for (int col=0; col<cols; col++) {
			if (board[row+1][col+1] != 0) {
				c++;
			}
		}
	}
	return c;
}

int is_legal_relationship(const int nv, const int ev, const int edr, const int edc) {
	// nv => new value
	// ev => existing value
	// edr => row delta of existing value
	// edc => col  delta of existing value
	if ((0>nv) || (nv>2)) {
		fprintf(stderr, "Invalid new value for is_legal_relationship\n");
		exit(1);
	}
	printf("%d, %d, %d, %d\n", nv, ev, edr, edc);
	if (nv == 0 || ev == 0) return 1;
	if (edr == 0 && edc == 0) return 1;
	if (edr == edc) return (nv == 2) ? 1 : (ev != 1);
	if (edr == -edc) return (nv == 1) ? 1: (ev != 2);
	return (nv == ev);
}

int is_legal_placement(const int rows, const int cols, int board[rows+2][cols+2], const int row, const int col) {
	int v; // Value at board[row][col]
	v = board[row][col];
	for (int dr=-1; dr<=1; dr++) {
		for (int dc=-1; dc<=1; dc++) {
			if (!is_legal_relationship(v, board[row+1+dr][col+1+dc], dr, dc)) return 0;
		}
	}
	return 1;
}

int solve(const int rows, const int cols, int board[rows+2][cols+2], int row, int col, const int diags) {
	// board should be rows+2 by cols+2 because it is padded with zeros.
	if (count_nonzero(rows, cols, board) >= diags) return 1;
	for (int diagtype=2; diagtype>=0; diagtype--) {
		printf("a: %d %d\n", row, col);
		board[row][col] = diagtype;
		if (is_legal_placement(rows, cols, board, row, col)) {
			col = (col + 1) % cols;
			row = (col == 0) ? row + 1: 1;
			printf("b: %d %d\n", row, col);
			if (row == rows) return 0; // No solutions exist.
			print_board(rows, cols, board);
			solve(rows, cols, board, row, col, diags);
			print_board(rows, cols, board);
		} else printf("Nope!\n");
	}
	printf("Got here!\n");
	print_board(rows, cols, board);
	return 0;
}


int main(int argc, char *argv[]) {
	int r;

	// if (argc != 4) {
	// 	fprintf(stderr, "Usage: ndiagonals <Rows> <Cols> <Diagonals>\n");
	// 	exit(1);
	// }
	// for (int i=1; i<4; i++) {
	// 	if (atoi(argv[i]) <=0) {
	// 		fprintf(stderr, "Error: All arguments must be positive\n");
	// 		exit(1);
	// 	}
	// }

	const int rows =  5;//atoi(argv[1]);
	const int cols =  5;//atoi(argv[2]);
	const int diags = 16;//atoi(argv[3]);
	int board[rows+2][cols+2];
	memset(board, 0, sizeof(int) * (rows + 2) * (cols + 2));

	if ((r = solve(rows, cols, board, 1, 1, diags))) {
		printf("Success!\n");
		print_board(rows, cols, board);
	} else printf("No solution found!\n");
	return r;
}