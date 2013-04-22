#ifndef _DANCING_LINKS_H
#define _DANCING_LINKS_H

#include <stdint.h>
#include <string.h>
#include <limits.h>

#ifndef INT_MAX
#define	INT_MAX		2147483647
#endif
#define HEADER_ID	INT_MAX

struct dlx_node {
	struct dlx_node *lx;
	struct dlx_node *rx;
	void *ux;
	void *dx;
	void *cx;
	struct dlx_row *rowx;
	int row_id;
	int col_id;
};

struct dlx_column {
	void *lx;
	void *rx;
	void *ux;
	void *dx;
	void *cx;
	int s;		/* S[y] */
	int id;		/* N[y] */
};

struct dlx_row {
	void *lx;
	void *rx;
	int s;
	int id;
};

struct dlx_header {
	void *lx;
	void *rx;
	void *ux;
	void *dx;
	void *cx;
	int s;		/* S[y] */
	int id;
	void *pcell;
	void *pcol;
	void *prow;
	int col_num;
	int row_num;
	int size;
};

struct solve_result {
	int num;
	int index;
	int *data;
};

struct location {
	int r;
	int c;
};

struct dlx_matrix {
	int col_num;
	int row_num;
	int size;
	struct location *bitset;
};


void show_solution(struct solve_result *resule);
struct dlx_column *min_s_col(const struct dlx_header *h);
void dlx_search(struct dlx_header *h, struct solve_result *result);
struct dlx_matrix *alloc_matrix_via_str(struct dlx_matrix *matrix, const char *str, int col_num, int row_num);
void free_matrix(struct dlx_matrix *matrix);
int how_many_1_in_str(const char *str);
void print_dlx_matrix(const struct dlx_matrix *matrix);
void haddvialocation(struct dlx_header *h, const struct location *loc);
struct dlx_header *matrix2h(struct dlx_header *h, const struct dlx_matrix *matrix);
char *h2str(char **str, const struct dlx_header *h);
void free_str(char *str);
struct dlx_matrix *h2matrix(struct dlx_matrix *matrix, const struct dlx_header *h);
void free_dlx_h(struct dlx_header *h);

void dlx_cover_col(struct dlx_column *col);
void dlx_uncover_col(struct dlx_column *col);
#endif

/* vim: set ts=8 sw=8 tw=78 ai si: */
