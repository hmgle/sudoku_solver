#include "dancing_links.h"
#include "debug_print.h"
#include <stdlib.h>

int main(int argc, char **argv)
{
	int row_num;
	int col_num;
	struct dlx_matrix matrix;
	struct dlx_matrix test_matrix;
	struct dlx_header h;
	char *matrix_dsp = NULL;
	struct solve_result result;

	if (argc < 4)
		return -1;
	row_num = atoi(argv[1]);
	col_num = atoi(argv[2]);

	alloc_matrix_via_str(&matrix, argv[3], col_num, row_num);
	print_dlx_matrix(&matrix);


	matrix2h(&h, &matrix);
	h2str(&matrix_dsp, &h);

	debug_print("matrix_dsp is %s", matrix_dsp);

	dlx_search(&h, &result);

	h2matrix(&test_matrix, &h);
	print_dlx_matrix(&test_matrix);

	free_str(matrix_dsp);
	free_dlx_h(&h);
	free_matrix(&test_matrix);
	free_matrix(&matrix);
	return 0;
}
