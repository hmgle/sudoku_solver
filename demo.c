#include <stdio.h>
#include <stdlib.h>
#include "dancing_links.h"
#include "debug_print.h"

#define MAX_COLUMN	324	/* 81 cells + (9 + 9 + 9) * 9 */
#define MAX_ROW		729	/* 81 cells * 9  */
#define SUDOKU_RANK	9

int count = 0;
/* struct dlx_header h; */
struct dlx_column h;
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
void search(int n);
void print_solution(void);
/* struct dlx_column *choose_min_col(struct dlx_header *header); */
struct dlx_column *choose_min_col(struct dlx_column *header);
void set_cell(int val, int row, int col);
void solution_row(int row);
void cover_col(struct dlx_column *col);
void uncover_col(struct dlx_column *col);
void add_solutions(struct dlx_node *row);
void remove_solutions(struct dlx_node *row);

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
	h.id = 77777;
	col[0].lx = &h;
	col[0].rx = &col[1];
	col[0].cx = &col[0];
	col[0].s = SUDOKU_RANK;
	col[0].id = 0;
	col[MAX_COLUMN - 1].rx = &h;
	col[MAX_COLUMN - 1].lx = &col[MAX_COLUMN - 2];
	col[MAX_COLUMN - 1].cx = &col[MAX_COLUMN - 1];
	col[MAX_COLUMN - 1].s = SUDOKU_RANK;
	col[MAX_COLUMN - 1].id = MAX_COLUMN - 1;
	for (i = 1; i < MAX_COLUMN - 1; i++) {
		col[i].rx = &col[i + 1];
		col[i].lx = &col[i - 1];
		col[i].cx = &col[i];
		col[i].s = SUDOKU_RANK;
		col[i].id = i;
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
		solution_row(row * SUDOKU_RANK * SUDOKU_RANK 
				+ col * SUDOKU_RANK + val - 1);
}

void solution_row(int row)
{
	struct dlx_node *tmp_cell;
	struct dlx_column *tmp_column;

	tmp_cell = &cell[select_col0_by_row(row)][select_row0_by_row(row)];
	tmp_column = (struct dlx_column *)(tmp_cell->cx);
	debug_print();
	cover_col(tmp_column);

	debug_print("select_row1_by_row(row) = %d ", select_row1_by_row(row));
	debug_print("select_col1_by_row(row) = %d ", select_col1_by_row(row));
	tmp_cell = &cell[select_col1_by_row(row)][select_row1_by_row(row)];
	tmp_column = (struct dlx_column *)(tmp_cell->cx);
	debug_print();
	cover_col(tmp_column);

	tmp_cell = &cell[select_col2_by_row(row)][select_row2_by_row(row)];
	tmp_column = (struct dlx_column *)(tmp_cell->cx);
	debug_print();
	cover_col(tmp_column);

	tmp_cell = &cell[select_col3_by_row(row)][select_row3_by_row(row)];
	tmp_column = (struct dlx_column *)(tmp_cell->cx);
	debug_print();
	cover_col(tmp_column);

	return;
}

void cover_col(struct dlx_column *col)
{
	struct dlx_node *row;
	struct dlx_node *rightnode;

	count++;
	debug_print("id is %d", col->id);
	debug_print("col->lx id is %d", ((struct dlx_column *)(col->lx))->id);
	debug_print("col->lx id is %d", ((struct dlx_column *)(col->rx))->id);
	((struct dlx_column *)(col->lx))->rx = col->rx;
	((struct dlx_column *)(col->rx))->lx = col->lx;

	for (row = (struct dlx_node *)(col->dx); 
			row != (struct dlx_node *)col; 
			row = (struct dlx_node *)(row->dx)) {
		for (rightnode = row->rx; 
			rightnode != row; 
			rightnode = rightnode->rx) {
			((struct dlx_node *)(rightnode->ux))->dx = 
				rightnode->dx;
			((struct dlx_node *)(rightnode->dx))->ux = 
				rightnode->ux;
			((struct dlx_column *)rightnode->cx)->s -= 1;
		}
	}
	return;
}

void uncover_col(struct dlx_column *col)
{
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

void search(int n)
{
	struct dlx_column *min_col;
	struct dlx_node *row;
	struct dlx_node *rightnode;
	struct dlx_node *leftnode;

	if (h.rx == &h) {
		debug_print();
		print_solution();
		return;
	} else {
		min_col = choose_min_col(&h);
		debug_print("min_col->id is %d", min_col->id);
		cover_col(min_col);

		for (row = min_col->dx; row != (void *)min_col; row = row->dx) {
			add_solutions(row);

			for (rightnode = row->rx; 
				rightnode != row;
				rightnode = rightnode->rx) {
				debug_print();
				cover_col(rightnode->cx);
			}

			search(n + 1);

			remove_solutions(row);
			// min_col = row->cx;

			for (leftnode = row->lx;
				leftnode != row;
				leftnode = leftnode->lx)
				uncover_col(leftnode->cx);
		}
		uncover_col(min_col);
	}
	return;
}

void print_solution(void)
{
	return;
}

/* struct dlx_column *choose_min_col(struct dlx_header *header) */
struct dlx_column *choose_min_col(struct dlx_column *header)
{
	struct dlx_column *min_col;
	struct dlx_column *tmp_col;

	if (header->rx == header)
		return NULL;
	min_col = (struct dlx_column *)(header->rx);
	for (tmp_col = min_col->rx;
		tmp_col != (void *)header;
		tmp_col = tmp_col->rx) {
		if (tmp_col->s < min_col->s)
			min_col = tmp_col;
	}
	return min_col;
}

void add_solutions(struct dlx_node *row)
{
	return;
}

void remove_solutions(struct dlx_node *row)
{
	debug_print("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
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
	search(0);

	release_sudoku();
	return 0;
}

/* vim: set ts=8 sw=8 tw=78 ai si: */
