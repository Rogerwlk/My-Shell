NAME = myshell

SRC = history.c mylib.c myshell.c

.PHONY: all clean fclean re

all: $(NAME)

$(NAME):
	gcc -o $(NAME) -Wall -Wextra $(SRC)

clean:
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re: fclean all