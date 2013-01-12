#ifndef _DANCING_LINKS_H
#define _DANCING_LINKS_H

#include <stdint.h>
#include <string.h>

struct dlx_node {
	struct dlx_node *lx;
	struct dlx_node *rx;
	void *ux;
	void *dx;
	void *cx;
	int row_id;
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

struct dlx_header {
	void *lx;
	void *rx;
	void *ux;
	void *dx;
	void *cx;
	int s;		/* S[y] */
	int id;
};

#endif

/* vim: set ts=8 sw=8 tw=78 ai si: */
