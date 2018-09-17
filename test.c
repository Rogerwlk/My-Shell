/*
 * test.c
 *
 *  Created on: Jan 23, 2018
 *      Author: roger
 */

#include <stdlib.h>
#include <stdio.h>
#include "myshell.h"

int main(void) {
	char *a = "echo hello &<  >123 <  353";
	int num;
	char *in = NULL, *out = NULL;
	char **r = command_parser(a, &num);
	// printf("num=%d\n", num);


	int bg_flag;
	find_io(r, &in, &out, &bg_flag);

	char **temp = r;
	while(*temp) {
		printf("%s\n", *temp);
		temp++;
	}
	// printf("%s\n%s\n%d\n", in, out, bg_flag);

	return (0);
}
