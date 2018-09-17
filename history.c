/*
 * history.c
 *
 *  Created on: Jan 31, 2018
 *      Author: Roger Wang
 */

#include "myshell.h"

static const int HIST_SIZE = 4;

void print_history(const t_hist *hist_p) {
	if (hist_p->oversize_flag) {
		for (int i = 0; i < HIST_SIZE; i++) {
			printf("%d  %s\n", i, hist_p->arr[(hist_p->idx + i) % HIST_SIZE]);
		}
	} else {
		for (int i = 0; i < hist_p->idx; i++) {
			printf("%d  %s\n", i, hist_p->arr[i]);
		}
	}
}

void clear_history(t_hist *hist_p) {
	for (int i = 0; i < HIST_SIZE; i++) {
		if ((hist_p->arr)[i]) {
			free((hist_p->arr)[i]);
			(hist_p->arr)[i] = NULL;
		}
	}
	hist_p->idx = 0;
	hist_p->oversize_flag = 0;
}
