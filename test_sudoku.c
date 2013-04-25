#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dlx.h"
#include "debug_print.h"

#define MAX_COLUMN	324	/* 81 cells + (9 + 9 + 9) * 9 */
#define MAX_ROW		729	/* 81 cells * 9  */
#define SUDOKU_RANK	9

static int select_col0_by_row(int row)
{
	return row / SUDOKU_RANK;
}

static int select_col1_by_row(int row)
{
	return SUDOKU_RANK * SUDOKU_RANK 
		+ row / (SUDOKU_RANK * SUDOKU_RANK) * SUDOKU_RANK 
		+ row % SUDOKU_RANK;
}

static int select_col2_by_row(int row)
{
	return SUDOKU_RANK * SUDOKU_RANK * 2 
		+ row % (SUDOKU_RANK * SUDOKU_RANK);
}

static int select_col3_by_row(int row)
{
	return SUDOKU_RANK * SUDOKU_RANK * 3 
		+ (row / (SUDOKU_RANK * SUDOKU_RANK * 3)) * SUDOKU_RANK * 3 
		+ (row % (SUDOKU_RANK * SUDOKU_RANK * 3)) 
		% (SUDOKU_RANK * SUDOKU_RANK) / (SUDOKU_RANK * 3)
		* SUDOKU_RANK
		+ row % SUDOKU_RANK;
}

static struct dlx_node *find_row_node(struct dlx_head *h, int row)
{
	struct dlx_node *node;
	struct dlx_node *col;
	int col_id;

	col_id = select_col0_by_row(row);
	for (col = h->h.rx; col != &h->h; col = col->rx) {
		if (col->colx->id == col_id) {
			for (node = col->dx; node != col; node = node->dx) {
				if (node->row_id == row) {
					return node;
				}
			}
		}
	}
	return NULL;
}

int main(void)
{
	char input[MAX_COLUMN * MAX_ROW + 1];
	size_t i;

	int row_num;
	int col_num;
	struct dlx_matrix matrix;
	struct dlx_head dlx_h;
	int is_run = 1;
	int n;
	int *solution;
	struct dlx_node *sel_row[MAX_ROW];

	memset(sel_row, 0, sizeof(sel_row));
	memset(&input, 0, sizeof(input));
#if 1
	for (i = 0; i < MAX_COLUMN * MAX_ROW; i++) {
		input[i] = '0';
	}
	for (i = 0; i < MAX_ROW; i++) {
		int loc;
		loc = i * MAX_COLUMN + select_col0_by_row(i);
		input[loc] = '1';
		loc = i * MAX_COLUMN + select_col1_by_row(i);
		input[loc] = '1';
		loc = i * MAX_COLUMN + select_col2_by_row(i);
		input[loc] = '1';
		loc = i * MAX_COLUMN + select_col3_by_row(i);
		input[loc] = '1';
	}
#else
	srand((unsigned int)time(NULL));
	int count = 0;
	for (i = 0; i < MAX_COLUMN * MAX_ROW; i++) {
		// input[i] = '1';
		// input[i] = rand() % 2 + '0';
		input[i] = rand() % 47 == 0 ? '1' : '0';
		if (input[i] == '1')
			count++;
	}
	debug_print("count is %d", count);
#endif

	col_num = MAX_COLUMN;
	row_num = MAX_ROW;
	solution = malloc(sizeof(int) * row_num);
	memset(solution, 0, sizeof(int) * row_num);
	memset(&matrix, 0, sizeof(struct dlx_matrix));
	alloc_matrix_via_str(&matrix, input, col_num, row_num);
	memset(&dlx_h, 0, sizeof(struct dlx_head));
	dlx_header_init(&dlx_h, col_num, row_num);
	matrix_to_header(&dlx_h, &matrix);

	sel_row[0] = find_row_node(&dlx_h, 1);
	sel_row[1] = find_row_node(&dlx_h, 2);
	// dlx_select_row(sel_row[0]);
	dlx_select_row(sel_row[1]);

	/* for test search */
	n = dlx_search(&dlx_h, solution, 0, &is_run);
	debug_print("dlx_search return %d", n);
	/* end for test search */
	if (n > 0) {
		printf("dlx_search() return %d\n", n);
		for (i = 0; i < (size_t)n; i++) {
			// debug_print("------- %d ", solution[i]);
			printf("------- %d \n", solution[i]);
		}
	}

	for (i = 0; i < sizeof(sel_row) / sizeof(sel_row[0]); i++) {
		if (sel_row[i]) {
			dlx_unselect_row(sel_row[i]);
		}
	}

	dlx_header_release(&dlx_h);
	free_matrix(&matrix);
	free(solution);
	return 0;
}
