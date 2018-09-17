#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char **argv) {
	int fd[2];
	pid_t childpid;

	if (pipe(fd) == -1) {
		printf("error\n");
		exit(1);
	}

	if ((childpid = fork()) == -1) {
		perror("fork\n");
		exit(1);
	}

	if(childpid == 0) {
		// Child process close stdout, duplicate output side of pipe to stdout
		dup2(1, fd[0]);
		execlp(argv[1], argv[1], argv[2], NULL);
	}
	else {
		// Parent process closes up output side of pipe
		wait(NULL);
		// fprintf(stderr,"finish part1\n");
	}
	// perror("error:");
	// return(0);
	if ((childpid = fork()) == -1) {
		perror("fork\n");
		exit(1);
	}

	if(childpid == 0) {
		// Child process close stdout, duplicate output side of pipe to stdout
		dup2(0, fd[1]);
		execlp(argv[4], argv[4], argv[5],NULL);
	}
	else {
		// Parent process closes up output side of pipe
		// close(fd[1]);
		wait(NULL);
		// perror("finish part2\n");
	}

	// int rc = fork();
	// if (rc < 0) {
	// 	fprintf(stderr, "Fork failed.\n");
	// 	printf("Ending parent process PID: %d\n", getpid());
	// 	return EXIT_FAILURE;
	// } else if (rc == 0) {
	// 	printf("Starting child process PID: %d\n", getpid());
	// 	fclose(1);
	// 	execlp(argv[0], argv[0], argv[1], NULL);
	// } else {
	// 	num--;
	// }
	return 0;
}
