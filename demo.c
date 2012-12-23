#include <stdio.h>
#include <stdlib.h>
#include "dancing_links.h"

#define MAX_COLUMN	324	/* 81 cells + (9 + 9 + 9) * 9 */
#define MAX_ROW		729	/* 81 cells * 9  */
#define SUDOKU_MAXNUM	9

struct dlx_header h;
struct dlx_column col[MAX_COLUMN];
struct dlx_node cell[MAX_COLUMN][SUDOKU_MAXNUM];

void show_usage(char *name);
void init_sudoku(void);
void release_sudoku(void);

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
	col[MAX_COLUMN - 1].rx = &h;
	col[MAX_COLUMN - 1].lx = &col[MAX_COLUMN - 2];
	for (i = 1; i < MAX_COLUMN - 2; i++) {
		col[i].rx = &col[i + 1];
		col[i].lx = &col[i - 1];
	}

	/*
	 * link echo col
	 */
	for (i = 0; i < MAX_COLUMN; i++) {
		col[i].dx = &cell[i][0];
		col[i].ux = &cell[i][SUDOKU_MAXNUM - 1];
		cell[i][0].ux = &col[i];
		cell[i][SUDOKU_MAXNUM - 1].ux = &col[i];
		for (j = 1; j < SUDOKU_MAXNUM - 1; j++) {
			cell[i][j].dx = &cell[i][j + 1];
			cell[i][j].ux = &cell[i][j - 1];
		}
	}

	/*
	 * link echo row
	 */
	for (i = 0; i < MAX_ROW; i++) {

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

int main(int argc, char **argv)
{
	char *this_name = argv[0];
	char *thisarg;
	char inputfilename[128] = "";

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

	init_sudoku();

	release_sudoku();
	return 0;
}

/* vim: set ts=8 sw=8 tw=78 ai si: */
