#include "dancing_links.h"
#include "debug_print.h"
#include <stdlib.h>
#include <assert.h>

void print_dlx_matrix(const struct dlx_matrix *matrix)
{
	int i, j;
	int row, col;
	int *data;

	data = malloc(sizeof(int) * matrix->row_num * matrix->col_num);
	memset(data, 0, sizeof(int) * matrix->row_num * matrix->col_num);
	for (i = 0; i < matrix->size; i++) {
		row = matrix->bitset[i].r;
		col = matrix->bitset[i].c;
		*(data + row * matrix->col_num + col) = 1;
	}

	printf("row num = %d, col num = %d\n", matrix->row_num, matrix->col_num);
	for (i = 0; i < matrix->row_num; i++) {
		for (j = 0; j < matrix->col_num; j++) {
			printf("%d ", *(data + i * matrix->col_num + j));
		}
		printf("\n");
	}
	free(data);
}

int how_many_1_in_str(const char *str)
{
	int ret = 0;
	int len = strlen(str);
	int i;

	for (i = 0; i < len; i++)
		if (str[i] == '1')
			ret++;
	return ret;
}

struct dlx_matrix *alloc_matrix_via_str(struct dlx_matrix *matrix, const char *str, int col_num, int row_num)
{
	int i;
	int len = strlen(str);
	int index = 0;

	matrix->col_num = col_num;
	matrix->row_num = row_num;
	matrix->size = how_many_1_in_str(str);
	matrix->bitset = malloc(sizeof(struct location) * matrix->size);
	for (i = 0; i < len; i++) {
		if (str[i] == '1') {
			matrix->bitset[index].r = i / col_num;
			matrix->bitset[index++].c = i % col_num;
		}
	}
	return matrix;
}

void free_matrix(struct dlx_matrix *matrix)
{
	if (matrix->bitset) {
		free(matrix->bitset);
		matrix->bitset = NULL;
	}
}

struct dlx_header *matrix2h(struct dlx_header *h, const struct dlx_matrix *matrix)
{
	struct dlx_column *col;
	struct dlx_node *cell;
	int i, j;
	struct dlx_node tmp_row;

	memset(h, 0, sizeof(struct dlx_header));
	col = malloc(sizeof(struct dlx_column) * matrix->col_num);
	memset(col, 0, sizeof(struct dlx_column) * matrix->col_num);
	cell = malloc(sizeof(struct dlx_node) * matrix->size);
	memset(cell, 0, sizeof(struct dlx_node) * matrix->size);
	h->row_num = matrix->row_num;
	h->col_num = matrix->col_num;
	h->size = matrix->size;
	h->rx = &col[0];
	h->lx = &col[matrix->col_num - 1];
	h->pcell = cell;
	h->pcol = col;
	h->id = HEADER_ID;
	col[0].lx = h;
	col[matrix->col_num - 1].rx = h;
	col[matrix->col_num - 1].dx = &col[matrix->col_num - 1];
	col[matrix->col_num - 1].ux = &col[matrix->col_num - 1];
	col[matrix->col_num - 1].cx = &col[matrix->col_num - 1];
	col[matrix->col_num - 1].s = 0;
	col[matrix->col_num - 1].id = matrix->col_num - 1;
	for (i = 0; i < matrix->col_num - 1; i++) {
		col[i].rx = &col[i + 1];
		col[i + 1].lx = &col[i];
		col[i].dx = &col[i];
		col[i].ux = &col[i];
		col[i].cx = &col[i];
		col[i].s = 0;
		col[i].id = i;
	}
	for (i = 0; i < matrix->size; i++) {
		cell[i].row_id = matrix->bitset[i].r;
		cell[i].col_id = matrix->bitset[i].c;
		cell[i].cx = &col[i];
	}
	for (i = 0; i < matrix->col_num; i++) {
		for (j = 0; j < matrix->size; j++) {
			if (cell[j].col_id == i) {
				col[i].s++;
				((struct dlx_node *)col[i].ux)->dx = &cell[j];
				cell[j].ux = col[i].ux;
				cell[j].dx = &col[i];
				col[i].ux = &cell[j];
			}
		}
	}
	for (i = 0; i < matrix->row_num; i++) {
		tmp_row.rx = &tmp_row;
		tmp_row.lx = &tmp_row;
		for (j = 0; j < matrix->size; j++) {
			if (cell[j].row_id == i) {
				tmp_row.lx->rx = &cell[j];
				cell[j].lx = tmp_row.lx;
				cell[j].rx = &tmp_row;
				tmp_row.lx = &cell[j];
			}
		}
		tmp_row.rx->lx = tmp_row.lx;
		tmp_row.lx->rx = tmp_row.rx;
	}
	return h;
} /* matrix2h() */

char *h2str(char **str, const struct dlx_header *h)
{
	struct dlx_column *col;
	struct dlx_node *node;

	*str = malloc(sizeof(char) * (h->col_num * h->row_num + 1));
	memset(*str, '0', sizeof(char) * h->col_num * h->row_num);
	(*str)[h->col_num * h->row_num + 1] = '\0';
	assert(h);
	for (col = h->rx; col != (void *)h; col = col->rx) {
		for (node = col->dx; node != (void *)col; node = node->dx) {
			(*str)[node->row_id * h->col_num + node->col_id] = '1';
		}
	}
	return *str;
}

void free_str(char *str)
{
	if (str) {
		free(str);
		str = NULL;
	}
}

struct dlx_matrix *h2matrix(struct dlx_matrix *matrix, const struct dlx_header *h)
{
	char *matrix_dsp = NULL;

	matrix->row_num = h->row_num;
	matrix->col_num = h->col_num;
	matrix->size = h->size;
	matrix->bitset = malloc(sizeof(struct location) * matrix->size);

	assert(h);
	h2str(&matrix_dsp, h);
	alloc_matrix_via_str(matrix, matrix_dsp, h->col_num, h->row_num);
	free_str(matrix_dsp);
	return matrix;
}

void free_dlx_h(struct dlx_header *h)
{
	if (h->pcell) {
		free(h->pcell);
		h->pcell = NULL;
	}
	if (h->pcol) {
		free(h->pcol);
		h->pcol = NULL;
	}
}

struct dlx_column *min_s_col(const struct dlx_header *h)
{
	struct dlx_column *min_col;
	struct dlx_column *tmp_col;

	struct dlx_matrix test_matrix;

	h2matrix(&test_matrix, h);
	print_dlx_matrix(&test_matrix);

	if (h->rx == h)
		return NULL;
	min_col = h->rx;
	for (tmp_col = min_col->rx;
		tmp_col != (void *)h;
		tmp_col = tmp_col->rx) {
		if (tmp_col->s < min_col->s)
			min_col = tmp_col;
	}
	debug_print("mid_col id is %d", min_col->id);
	free_matrix(&test_matrix);
	return min_col;
}
void show_solution(struct solve_result *resule)
{
	debug_print();
}

void dlx_cover_col(struct dlx_column *col)
{
	struct dlx_node *row;
	struct dlx_node *rightnode;

	debug_print();
	((struct dlx_column *)(col->rx))->lx = col->lx;
	debug_print();
	((struct dlx_column *)(col->lx))->rx = col->rx;

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
			((struct dlx_column *)rightnode->cx)->s--;
		}
	}
	return;
}

void dlx_uncover_col(struct dlx_column *col)
{
	struct dlx_node *row;
	struct dlx_node *leftnode;

	for (row = (struct dlx_node *)(col->ux); 
			row != (struct dlx_node *)col; 
			row = (struct dlx_node *)(row->ux)) {
		for (leftnode = row->lx; 
			leftnode != row; 
			leftnode = leftnode->rx) {
			((struct dlx_column *)leftnode->cx)->s++;
			((struct dlx_node *)(leftnode->ux))->dx = leftnode;
			((struct dlx_node *)(leftnode->dx))->ux = leftnode;
		}
	}
	((struct dlx_column *)(col->rx))->lx = col;
	((struct dlx_column *)(col->lx))->rx = col;
	return;
}

void dlx_search(struct dlx_header *h, struct solve_result *result)
{
	struct dlx_column *min_col;
	struct dlx_node *node;
	struct dlx_node *rightnode;
	struct dlx_node *leftnode;

	struct dlx_matrix test_matrix;

	if (h->rx == h) {
		show_solution(result);
		return;
	} else {
		min_col = min_s_col(h);
		if (min_col->s == 0)
			return;

		dlx_cover_col(min_col);

		for (node = min_col->dx; node != (void *)min_col; node = node->dx) {
			h2matrix(&test_matrix, h);
			print_dlx_matrix(&test_matrix);
			free_matrix(&test_matrix);
			debug_print("node->cx id is %d", ((struct dlx_column *)(node->cx))->id);
			for (rightnode = node->rx; 
				rightnode != node;
				rightnode = rightnode->rx) {
				debug_print("rightnode->cx id is %d", ((struct dlx_column *)(rightnode->cx))->id);
				dlx_cover_col(rightnode->cx);
			}
			h2matrix(&test_matrix, h);
			print_dlx_matrix(&test_matrix);
			free_matrix(&test_matrix);
			dlx_search(h, result);
			for (leftnode = node->lx;
				leftnode != node;
				leftnode = leftnode->lx) {
				dlx_uncover_col(leftnode->cx);
			}
		}

		dlx_uncover_col(min_col);
	}

	return;
}

/* vim: set ts=8 sw=8 tw=78 ai si: */
