/*
 * mylib.c
 *
 *  Created on: Feb 6, 2018
 *      Author: Roger Wang
 */

#include "myshell.h"

int command_count(char *str, const char split);
char *command_create(char **str, const char split);

char **line_parser(char *str, int *num, const char split) {
	char **arr;

	*num = command_count(str, split);
	arr = (char **) malloc(sizeof(char *) * (*num + 1));
	for (int i = 0; i < *num; i++) {
		arr[i] = command_create(&str, split);
	}
	arr[*num] = 0;
	return arr;
}

int command_count(char *str, const char split) {
	int count = 1;
	while (*str) {
		if (*str == split) {
			count++;
		}
		str++;
	}
	return count;
}

char *command_create(char **str, const char split) {
	char *command;
	int len = 0;

	while (**str && **str == ' ') {
		(*str)++;
	}
	while ((*str)[len] && (*str)[len] != split) {
		len++;
	}
	command = (char *) malloc(sizeof(char) * (len + 1));
	for (int i = 0; i < len; i++) {
		command[i] = **str;
		(*str)++;
	}
	command[len] = 0;
	if (**str == split)
		(*str)++;
	return command;
}

int word_count(char *str);
char *word_create(char **str);
char *char_create(char **ch);
char *file_create(char **str);

void skip_space(char **str) {
	while (**str == ' ') {
		(*str)++;
	}
}

void skip_name(char **str) {
	skip_space(str);
	while (**str && **str != ' ' && **str != '&'
		&& **str != '<' && **str != '>') {
		(*str)++;
	}
}

char **command_parser(char *str, int *num) {
	char **arr;
	int i = 0;

	skip_space(&str);
	*num = word_count(str);
	arr = (char **) malloc(sizeof(char *) * (*num + 1));
	while (*str) {
		if (*str == '&')
			arr[i++] = char_create(&str);
		else if (*str == '<' || *str == '>')
			arr[i++] = file_create(&str);
		else
			arr[i++] = word_create(&str);
		skip_space(&str);
	}
	arr[*num] = 0;
	return arr;
}

int word_count(char *str) {
	int count = 0;
	while (*str) {
		if (*str == '&') {
			count++;
			str++;
		} else if (*str == '<' || *str == '>') {
			count++;
			str++;
			skip_name(&str);
		} else {
			count++;
			skip_name(&str);
		}
		skip_space(&str);
	}
	return count;
}

char *char_create(char **ch) {
	char *word;
	word = (char *) malloc(sizeof(char) * 2);
	word[0] = **ch;
	word[1] = 0;
	(*ch)++;
	return word;
}

char *word_create(char **str) {
	int len = 0;
	char *word;
	while ((*str)[len] && (*str)[len] != ' ' && (*str)[len] != '<'
			&& (*str)[len] != '>' && (*str)[len] != '&') {
		len++;
	}
	word = (char *) malloc(sizeof(char) * (len + 1));
	for (int i = 0; i < len; i++) {
		word[i] = **str;
		(*str)++;
	}
	word[len] = 0;
	return word;
}

char *file_create(char **str) {
	int len = 0;
	char temp = **str;
	char *word;

	(*str)++;
	skip_space(str);
	while ((*str)[len] && (*str)[len] != ' ' && (*str)[len] != '<'
		&& (*str)[len] != '>' && (*str)[len] != '&') {
		len++;
	}
	word = (char *) malloc(sizeof(char) * (len + 2));
	word[0] = temp;
	for (int i = 1; i <= len; i++) {
		word[i] = **str;
		(*str)++;
	}
	word[len + 1] = 0;
	return word;
}

void remove_one_string(char **str);
void find_io(char **copy, char **in, char **out, int *bg_flag) {
	char **str = copy;
	*bg_flag = 0;
	if (*in)
		free(*in);
	if (*out)
		free(*out);
	*in = *out = NULL;
	while (*str) {
		if (**str == '>') {
			if (*out)
				free(*out);
			*out = (char *) malloc(sizeof(char) * (strlen(*str)));
			strcpy(*out, (*str) + 1);
			remove_one_string(str);
		} else if (**str == '<') {
			if (*in)
				free(*in);
			*in = (char *) malloc(sizeof(char) * (strlen(*str)));
			strcpy(*in, (*str) + 1);
			remove_one_string(str);
		} else if (**str == '&') {
			*bg_flag = 1;
			remove_one_string(str);
		} else
			str++;
	}
}

void remove_one_string(char **str) {
	if (!str[0]) {
		fprintf(stderr, "error: try to remove an item not exist.\n");
		exit(EXIT_FAILURE);
	}
	free(str[0]);
	while (str[0]) {
		str[0] = str[1];
		str++;
	}
}

int is_equal_string(const char *str1, const char *str2) {
	while (*str1 && *str2) {
		if (*str1 == *str2) {
			str1++;
			str2++;
		} else
			return 0;
	}
	if (*str1 == 0 && *str2 == 0)
		return 1;
	else
		return 0;
}

int is_numeric(char *str) {
	while (*str == ' ')
		str++;
	if (*str == '+' || *str == '-')
		str++;
	while (*str) {
		if (*str < '0' || *str > '9')
			return 0;
		str++;
	}
	return 1;
}

void free_memory(char **str) {
	if (str) {
		char **copy = str;
		while (*copy) {
			free(*copy);
			(*copy) = NULL;
			copy++;
		}
		free(str);
		str = NULL;
	}
}