/*
 ============================================================================
 Name        : myshell.c
 Author      : Roger Wang
 Description : shell program
 ============================================================================
 */

#include "myshell.h"

static const int HIST_SIZE = 100;
static const int LINE_SIZE = 2048;
static const char *PROMPT = "$ ";
static const char *EXIT_COMMAND = "exit";

int run_command(t_hist *hist, char *str);

void readline(char *line) {
	int i = 0;

	if (fgets(line, LINE_SIZE, stdin) == NULL) {
		perror("error");
		return;
	}
	while (i <= LINE_SIZE && line[i] != '\n') {
		i++;
	}
	while (i <= LINE_SIZE) {
		line[i++] = '\0';
	}
}

int main(void) {
	t_hist hist;
	hist.arr = (char **) malloc(sizeof(char *) * HIST_SIZE);
	for (int i = 0; i < HIST_SIZE; i++) {
		hist.arr[i] = NULL;
	}
	hist.idx = 0;
	hist.oversize_flag = 0;
	char *line = NULL;
	char **commands = NULL;
	int command_num = 0;
	int loop_flag = 1;

	do {
		// read one line
		line = (char *) malloc(sizeof(char) * (LINE_SIZE + 1)); // allocate new memory for storing a command line
		printf("%s", PROMPT);
		readline(line);

		// store into history
		if (hist.arr[hist.idx]) {
			free(hist.arr[hist.idx]);
			hist.arr[hist.idx] = NULL;
		}
		hist.arr[hist.idx++] = line;
		if (hist.idx == HIST_SIZE) {
			hist.oversize_flag = 1;
			hist.idx = 0;
		}

		// parse command by ;
		commands = line_parser(line, &command_num, ';');
		if (command_num == 0)
			continue;
		for (int i = 0; i < command_num; i++) {
			if (is_equal_string(commands[i], EXIT_COMMAND)) {
				loop_flag = 0;
				break;
			} else {
				if (run_command(&hist, commands[i]) == -2) {
					loop_flag = 0;
					break;
				}
			}
		}
		free_memory(commands);
		commands = NULL;
	} while (loop_flag);
	clear_history(&hist);
	free(hist.arr);
	hist.arr = NULL;
	return EXIT_SUCCESS;
}

int run_command(t_hist *hist, char *str) {
	int pipe_num;
	char **pipes = NULL;
	char **copy;
	int old_fds[2], new_fds[2];

	old_fds[0] = old_fds[1] = -1;
	pipes = line_parser(str, &pipe_num, '|');
	if (pipe_num == 0) {
		fprintf(stderr, "error: syntax error near unexpected token \'|\'\n");
		return EXIT_FAILURE;
	}
	copy = pipes;
	while (*copy) {
		char *in = NULL, *out = NULL;
		int list_num, bg_flag = 0;
		char **list;
		pid_t childpid;

		list = command_parser(*copy, &list_num);
		find_io(list, &in, &out, &bg_flag);
		if (copy[1])
		{
			if (bg_flag) {
				fprintf(stderr, "error: syntax error near unexpected token \'|\'\n");
				free_memory(list);
				list = NULL;
				free_memory(pipes);
				pipes = NULL;
				return EXIT_FAILURE;
			}
			if (pipe(new_fds) == -1) {
				perror("error");
				free_memory(list);
				list = NULL;
				free_memory(pipes);
				pipes = NULL;
				return EXIT_FAILURE;
			}
		}
		if ((childpid = fork()) == -1) {
			perror("error");
			free_memory(list);
			list = NULL;
			free_memory(pipes);
			pipes = NULL;
			return EXIT_FAILURE;
		}
		if (childpid == 0) {
			if (*(copy - 1)) {
				dup2(old_fds[0], 0);
				close(old_fds[0]);
				close(old_fds[1]);
			}
			if (copy[1]) {
				close(old_fds[0]);
				dup2(new_fds[1], 1);
				close(new_fds[1]);
			}
			if (in) {
				FILE *fpi = NULL; // file pointer for input
				if ((fpi = fopen(in, "r")) == NULL) {
					perror("error");
					free_memory(list);
					list = NULL;
					free_memory(pipes);
					pipes = NULL;
					return EXIT_FAILURE;
				}
				dup2(fileno(fpi), 0);
				fclose(fpi);
			}
			if (out) {
				FILE *fpo = NULL; // file pointer for output
				if ((fpo = fopen(out, "w")) == NULL) {
					perror("error");
					free_memory(list);
					list = NULL;
					free_memory(pipes);
					pipes = NULL;
					return EXIT_FAILURE;
				}
				dup2(fileno(fpo), 1);
				fclose(fpo);
			}
			if (is_equal_string(list[0], "history")) {
				if (list[1] == NULL)
					print_history(hist);
				else if (is_equal_string(list[1], "-c")) {
					clear_history(hist);
				} else if (is_numeric(list[1])) {
					int t = atoi(list[1]);
					if (hist->oversize_flag) {
						if (t < 0 || t >= HIST_SIZE) {
							fprintf(stderr, "error: history index out of bound.\n");
							free_memory(list);
							list = NULL;
							free_memory(pipes);
							pipes = NULL;
							return EXIT_FAILURE;
						}
					} else {
						if (t < 0 || t >= hist->idx) {
							fprintf(stderr, "error: history index out of bound.\n");
							free_memory(list);
							list = NULL;
							free_memory(pipes);
							pipes = NULL;
							return EXIT_FAILURE;
						}
					}
					if (run_command(hist, (*hist).arr[t]) == EXIT_FAILURE) {
						free_memory(list);
						list = NULL;
						free_memory(pipes);
						pipes = NULL;
						return EXIT_FAILURE;
					}
				} else {
					fprintf(stderr, "error: invalid number of index.\n");
					free_memory(list);
					list = NULL;
					free_memory(pipes);
					pipes = NULL;
					return EXIT_FAILURE;
				}
			} else if (is_equal_string(list[0], "cd")) {
				if (chdir(list[1]) == -1) {
					perror("error");
					free_memory(list);
					list = NULL;
					free_memory(pipes);
					pipes = NULL;
					return EXIT_FAILURE;
				}
			} else if (is_equal_string(list[0], EXIT_COMMAND)) {
				free_memory(list);
				list = NULL;
				free_memory(pipes);
				pipes = NULL;
				return -2;
			} else {
				if (execvp(list[0], list) < 0) {
					perror("error");
					free_memory(list);
					list = NULL;
					free_memory(pipes);
					pipes = NULL;
					return EXIT_FAILURE;
				}
			}
		} else {
			if (*(copy - 1)) {
				close(old_fds[0]);
				close(old_fds[1]);
			}
			if (copy[1]) {
				old_fds[0] = new_fds[0];
				old_fds[1] = new_fds[1];
			}
			if (!bg_flag) {
				wait(NULL);
			}
		}
		free_memory(list);
		list = NULL;
		copy++;
	}
	if (old_fds[0] != -1) {
		close(old_fds[0]);
		close(old_fds[1]);
	}
	free_memory(pipes);
	pipes = NULL;
	return EXIT_SUCCESS;
}