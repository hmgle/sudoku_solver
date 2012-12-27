#include <stdio.h>
#include <stdlib.h>
#include "dancing_links.h"
#include "debug_print.h"

#define MAX_COLUMN	324	/* 81 cells + (9 + 9 + 9) * 9 */
#define MAX_ROW		729	/* 81 cells * 9  */
#define SUDOKU_RANK	9

struct dlx_header h;
struct dlx_column col[MAX_COLUMN];
struct dlx_node cell[MAX_COLUMN][SUDOKU_RANK];

int select_col0_by_row(int row);
int select_row0_by_row(int row);
int select_col1_by_row(int row);
int select_row1_by_row(int row);
int select_col2_by_row(int row);
int select_row2_by_row(int row);
int select_col3_by_row(int row);
int select_row3_by_row(int row);
void init_sudoku(void);
void release_sudoku(void);
void show_usage(char *name);
void parse_input(int init_data[][SUDOKU_RANK], char *read_input, size_t size);
void print_soduku(int init_data[][SUDOKU_RANK]);
void set_sudoku(int init_data[][SUDOKU_RANK]);
void set_cell(int val, int row, int col);
void cover_row(int row);
void cover_col(int col);
void del_col(struct dlx_column *col);

int select_col0_by_row(int row)
{
	return row / SUDOKU_RANK;
}

int select_row0_by_row(int row)
{
	return row % SUDOKU_RANK;
}

int select_col1_by_row(int row)
{
	return SUDOKU_RANK * SUDOKU_RANK 
		+ row / (SUDOKU_RANK * SUDOKU_RANK) * SUDOKU_RANK 
		+ row % SUDOKU_RANK;
}

int select_row1_by_row(int row)
{
	return row % (SUDOKU_RANK * SUDOKU_RANK) / SUDOKU_RANK;
}

int select_col2_by_row(int row)
{
	return SUDOKU_RANK * SUDOKU_RANK * 2 
		+ row % (SUDOKU_RANK * SUDOKU_RANK);
}

int select_row2_by_row(int row)
{
	return row / (SUDOKU_RANK * SUDOKU_RANK);
}

int select_col3_by_row(int row)
{
	return SUDOKU_RANK * SUDOKU_RANK * 3 
		+ (row / (SUDOKU_RANK * SUDOKU_RANK * 3)) * SUDOKU_RANK * 3 
		+ (row % (SUDOKU_RANK * SUDOKU_RANK * 3)) 
		% (SUDOKU_RANK * SUDOKU_RANK) / (SUDOKU_RANK * 3)
		* SUDOKU_RANK
		+ row % SUDOKU_RANK;
}

int select_row3_by_row(int row)
{
	return (row % (SUDOKU_RANK * SUDOKU_RANK * 3)) 
		/ (SUDOKU_RANK * SUDOKU_RANK) * 3
		+ (row % (SUDOKU_RANK * 3)) / SUDOKU_RANK;
}

void init_sudoku(void)
{
	int i, j;

	/*
	 * link all col
	 */
	h.rx = &col[0];
	h.lx = &col[MAX_COLUMN - 1];
	col[0].lx = &h;
	col[0].rx = &col[1];
	col[0].cx = &col[0];
	col[MAX_COLUMN - 1].rx = &h;
	col[MAX_COLUMN - 1].lx = &col[MAX_COLUMN - 2];
	col[MAX_COLUMN - 1].cx = &col[MAX_COLUMN - 1];
	for (i = 1; i < MAX_COLUMN - 1; i++) {
		col[i].rx = &col[i + 1];
		col[i].lx = &col[i - 1];
		col[i].cx = &col[i];
	}

	/*
	 * link echo col
	 */
	for (i = 0; i < MAX_COLUMN; i++) {
		col[i].dx = &cell[i][0];
		col[i].ux = &cell[i][SUDOKU_RANK - 1];
		cell[i][0].dx = &cell[i][1];
		cell[i][0].ux = &col[i];
		cell[i][0].cx = &col[i];
		cell[i][SUDOKU_RANK - 1].dx = &col[i];
		cell[i][SUDOKU_RANK - 1].ux = &cell[i][SUDOKU_RANK - 2];
		cell[i][SUDOKU_RANK - 1].cx = &col[i];
		for (j = 1; j < SUDOKU_RANK - 1; j++) {
			cell[i][j].dx = &cell[i][j + 1];
			cell[i][j].ux = &cell[i][j - 1];
			cell[i][j].cx = &col[i];
		}
	}

	/*
	 * link echo row
	 */
	for (i = 0; i < MAX_ROW; i++) {
		cell[select_col0_by_row(i)][select_row0_by_row(i)].rx =
			&cell[select_col1_by_row(i)][select_row1_by_row(i)];
		cell[select_col0_by_row(i)][select_row0_by_row(i)].lx =
			&cell[select_col3_by_row(i)][select_row3_by_row(i)];

		cell[select_col1_by_row(i)][select_row1_by_row(i)].rx =
			&cell[select_col2_by_row(i)][select_row2_by_row(i)];
		cell[select_col1_by_row(i)][select_row1_by_row(i)].lx =
			&cell[select_col0_by_row(i)][select_row0_by_row(i)];

		cell[select_col2_by_row(i)][select_row2_by_row(i)].rx =
			&cell[select_col3_by_row(i)][select_row3_by_row(i)];
		cell[select_col2_by_row(i)][select_row2_by_row(i)].lx =
			&cell[select_col1_by_row(i)][select_row1_by_row(i)];

		cell[select_col3_by_row(i)][select_row3_by_row(i)].rx =
			&cell[select_col0_by_row(i)][select_row0_by_row(i)];
		cell[select_col3_by_row(i)][select_row3_by_row(i)].lx =
			&cell[select_col2_by_row(i)][select_row2_by_row(i)];
	}

	return;
}

void release_sudoku(void)
{
	return;
}

void show_usage(char *name)
{
	fprintf(stderr, "usage: %s [-i inputfile]\n", name);
	exit(1);
}

void parse_input(int init_data[][SUDOKU_RANK], char *read_input, size_t size)
{
	int i;
	int n = 0;

	for (i = 0; i < size; i++) {
		if (read_input[i] >= '1' && read_input[i] <= '9')
			*((*init_data)+ n) = read_input[i] - '0';
		else if (read_input[i] != '*' 
				&& read_input[i] != '.'
				&& read_input[i] != '0'
				&& read_input[i] != 'x'
				&& read_input[i] != 'X')
			continue;

		if (++n > SUDOKU_RANK * SUDOKU_RANK)
			break;
	}

	return;
}

void print_soduku(int init_data[][SUDOKU_RANK])
{
	int i, j;

	for (i = 0; i < SUDOKU_RANK; i++) {
		if (i % 3 == 0)
			printf("+-------+-------+-------+\n");
		for (j = 0; j < SUDOKU_RANK; j++) {
			if (j % 3 == 0)
				printf("| ");
			printf("%d ", init_data[i][j]);
			if (j == SUDOKU_RANK - 1)
				printf("|");
		}
		printf("\n");
	}
	printf("+-------+-------+-------+\n");
}

void set_cell(int val, int row, int col)
{
	if (val == 0)
		return;
	else
		cover_row(row * SUDOKU_RANK * SUDOKU_RANK 
				+ col * SUDOKU_RANK + val - 1);
}

void cover_row(int row)
{
	struct dlx_node *tmp_cell;
	struct dlx_column *tmp_column;

	tmp_cell = &cell[select_col0_by_row(row)][select_row0_by_row(row)];
	tmp_column = (struct dlx_column *)(tmp_cell->cx);
	del_col(tmp_column);

	debug_print("select_row1_by_row(row) = %d ", select_row1_by_row(row));
	debug_print("select_col1_by_row(row) = %d ", select_col1_by_row(row));
	tmp_cell = &cell[select_col1_by_row(row)][select_row1_by_row(row)];
	tmp_column = (struct dlx_column *)(tmp_cell->cx);
	del_col(tmp_column);

	tmp_cell = &cell[select_col2_by_row(row)][select_row2_by_row(row)];
	tmp_column = (struct dlx_column *)(tmp_cell->cx);
	del_col(tmp_column);

	tmp_cell = &cell[select_col3_by_row(row)][select_row3_by_row(row)];
	tmp_column = (struct dlx_column *)(tmp_cell->cx);
	del_col(tmp_column);

	return;
}

void cover_col(int col)
{
	return;
}

void del_col(struct dlx_column *col)
{
	((struct dlx_column *)(col->lx))->rx = col->rx;
	((struct dlx_column *)(col->rx))->lx = col->lx;
	return;
}

void set_sudoku(int init_data[][SUDOKU_RANK])
{
	int i, j;

	for (i = 0; i < SUDOKU_RANK; i++)
		for (j = 0; j < SUDOKU_RANK; j++)
			set_cell(init_data[i][j], i, j);

	return;
}

int main(int argc, char **argv)
{
	int init_data[SUDOKU_RANK][SUDOKU_RANK] = {{0}};
	char read_input[256] = {0};
	char *this_name = argv[0];
	char *thisarg;
	char inputfilename[128] = "";
	int ret;

	argc--;
	argv++;
	while (argc >= 1 && **argv == '-') {
		thisarg = *argv;
		thisarg++;
		switch (*thisarg) {
		case 'i':
			if (--argc <= 0)
				show_usage(this_name);
			argv++;
			strncpy(inputfilename, *argv, sizeof(inputfilename) - 1);
			break;
		default:
			show_usage(this_name);
		}
	}

	ret = scanf("%255s", read_input);
	parse_input(init_data, read_input, sizeof(read_input));

	print_soduku(init_data);

	init_sudoku();
	set_sudoku(init_data);

	release_sudoku();
	return 0;
}

/* vim: set ts=8 sw=8 tw=78 ai si: */
