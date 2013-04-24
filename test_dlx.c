#include <stdlib.h>
#include "dlx.h"

static void show_usage(const char *pro_name);

static void show_usage(const char *pro_name)
{
	fprintf(stderr, "Usage: %s row_num  col_num matrix_str\n", pro_name);
}

int main(int argc, char **argv)
{
	int row_num;
	int col_num;
	struct dlx_matrix matrix;
	struct dlx_head dlx_h;
	int is_run = 1;
	int n;

	if (argc < 4) {
		show_usage(argv[0]);
		return -1;
	}
	row_num = atoi(argv[1]);
	col_num = atoi(argv[2]);
	alloc_matrix_via_str(&matrix, argv[3], col_num, row_num);
	memset(&dlx_h, 0, sizeof(struct dlx_head));
	dlx_header_init(&dlx_h, col_num, row_num);
	matrix_to_header(&dlx_h, &matrix);

	n = dlx_search(&dlx_h, 0, &is_run);
	debug_print("dlx_search return %d", n);

	dlx_header_release(&dlx_h);
	free_matrix(&matrix);
	return 0;
}
