#include <stdio.h>

#define MAX_COLUMN	324	/* 81 cells + (9 + 9 + 9) * 9 */
#define MAX_ROW		729	/* 81 cells * 9  */
#define SUDOKU_MAXNUM	9

int col1(int row);
int row1(int row);
int col2(int row);
int row2(int row);
int col3(int row);
int row3(int row);
int col4(int row);
int row4(int row);

int col1(int row)
{
	return row / 9;
}

int row1(int row)
{
	return row % 9;
}

int col2(int row)
{
	return row / (9 * 9) * 9 + (row % (9 * 9)) % 9;
}

int row2(int row)
{
	return row % (9 * 9) / 9;
}

int col3(int row)
{
	return row % (9 * 9);
}

int row3(int row)
{
	return row / (9 * 9);
}

int col4(int row)
{
	return (row / (9 * 9 * 3)) * 9 * 3 
		+ (row % (9 * 9 * 3)) % (9 * 9) / (9 * 3) * 9
		+ row % 9;
}

int main(int argc, char **argv)
{
	int i;

	for (i = 0; i < MAX_ROW; i++) {
		printf("%d: %d\n", i, col4(i));
		// printf("%d: %d\n", i, row3(i));
	}

	return 0;
}
