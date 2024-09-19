NAME = philosophers
CC = cc 
CFLAGS = -fsanitize=thread -g3  -Wall -Wextra -Werror 


SOURCE = philosophers.c ft_start.c routine.c supervisor.c utils.c parsing.c

OBJ = $(SOURCE:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@ 

clean: 
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
