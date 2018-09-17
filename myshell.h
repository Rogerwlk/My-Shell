/*
 * myshell.h
 *
 *  Created on: Feb 6, 2018
 *      Author: Roger Wang
 */

#ifndef MYSHELL_H_
#define MYSHELL_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

typedef struct s_hist {
	char **arr;
	int idx;
	int oversize_flag;
} t_hist;

void print_history(const t_hist *hist_p);
void clear_history(t_hist *hist_p);

char **line_parser(char *str, int *num, const char split);
char **command_parser(char *str, int *num);
void find_io(char **str, char **in, char **out, int *bg_flag);
int is_numeric(char *str);
int is_equal_string(const char *str1, const char *str2);
void free_memory(char **str);

#endif /* MYSHELL_H_ */
