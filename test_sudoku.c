#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dlx.h"
#include "dlx_sudoku.h"
#include "debug_print.h"

static void init_sudoku_dlx_h(struct dlx_head *h)
{
	struct dlx_matrix matrix;
	char input[MAX_COLUMN * MAX_ROW + 1];
	size_t i;

	memset(&input, 0, sizeof(input));
	memset(&matrix, 0, sizeof(struct dlx_matrix));
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

	alloc_matrix_via_str(&matrix, input, MAX_COLUMN, MAX_ROW);
	matrix_to_header(h, &matrix);
	free_matrix(&matrix);
}

int main(void)
{
	size_t i;
	int row_num;
	int col_num;
	struct dlx_head dlx_h;
	int is_run = 1;
	int n;
	int *solution;
	struct dlx_node *sel_row[MAX_ROW];
	char read_input[256] = {0};
	struct sudoku_dsr sudoku;
	int ret;

	memset(sel_row, 0, sizeof(sel_row));

	col_num = MAX_COLUMN;
	row_num = MAX_ROW;
	solution = malloc(sizeof(int) * row_num);
	memset(solution, 0, sizeof(int) * row_num);
	memset(&dlx_h, 0, sizeof(struct dlx_head));
	dlx_header_init(&dlx_h, col_num, row_num);
	init_sudoku_dlx_h(&dlx_h);

	sudoku.data = malloc(sizeof(*sudoku.data) * SUDOKU_RANK	* SUDOKU_RANK);
	memset(sudoku.data, 0, sizeof(*sudoku.data) * SUDOKU_RANK * SUDOKU_RANK);
	ret = scanf("%255s", read_input);
	str2sudoku(&sudoku, SUDOKU_RANK, SUDOKU_RANK, read_input, sizeof(read_input));
	set_dlx_h_sudoku(&dlx_h, &sudoku, sel_row);
	print_sudoku(&sudoku);

	/* for test search */
	n = dlx_search(&dlx_h, solution, 0, &is_run);
	debug_print("dlx_search return %d", n);
	/* end for test search */
	if (n > 0) {
		printf("dlx_search() return %d\n", n);
		for (i = 0; i < (size_t)n; i++) {
			set_sudoku_cell_via_row(&sudoku, solution[i]);
		}
	}
	print_sudoku(&sudoku);

	/* for free */
	for (i = 0; i < sizeof(sel_row) / sizeof(sel_row[0]); i++) {
		if (sel_row[i]) {
			dlx_unselect_row(sel_row[i]);
		}
	}

	free(sudoku.data);
	dlx_header_release(&dlx_h);
	free(solution);
	return 0;
}
