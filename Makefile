TARGET = my_shell
OBJ = main.c input_parser.c helpers.c
CC = gcc

all:
	$(CC) -o $(TARGET) $(OBJ)
clean:
	re -f *-o
fclean: clear
	rm -f $(TARGET)
re: fclean all