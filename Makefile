NAME = poetry-parser

SRCS = srcs/main.c srcs/reading_parsing.c

FLAGS = -Wall -Werror -Wextra

all: $(NAME)

$(NAME):
	gcc -o $(NAME) $(FLAGS) $(SRCS)

clean:
	rm -f $(NAME)

fclean:
	rm -f $(NAME)

re: fclean all
