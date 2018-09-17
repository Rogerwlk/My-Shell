# My-Shell

The Makefile offers command "make", "make all", "make re", "make clean", "make fclean"

make: compile source files into a program called "myshell"
make all: same as above
make re: remove "myshell", obj files and recompile a new "myshell"
make clean: remove obj files
make fclean: remove obj files and "myshell"

The idea of the program is separating one line first by semicolon ';' into separate commands and then by '|' into parts. Before running each part, identify if it needs to be run in the background '&' or have any file redirection '<' '>'. Modify stdin and stdout if there is file redirection and pipe.