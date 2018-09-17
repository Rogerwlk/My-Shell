#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) {
	if (chdir("newdir") == -1) {
		perror("error");
		return EXIT_FAILURE;
	}
	return 0;
}