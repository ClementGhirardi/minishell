.PHONY: all clean fclean re

CC = cc

CFLAGS = -Wall -Wextra -Werror

NAME = minishell

NAME_BONUS = 

SRCS_DIR = srcs

SRCS_BONUS_DIR = srcs/srcs_bonus

SRCS = $(SRCS_DIR)/token.c \
	$(SRCS_DIR)/lexer.c \
	$(SRCS_DIR)/ast.c \
	$(SRCS_DIR)/parser.c \

SRCS_BONUS = 

MAIN = $(SRCS_DIR)/main.c

MAIN_BONUS = $(SRCS_BONUS_DIR)/main.o

INCLUDES = includes

LIBFT_DIR = libft

OBJS = $(SRCS:.c=.o)  $(MAIN:.c=.o)

OBJS_BONUS = $(SRCS_BONUS:.c=.o) $(MAIN_BONUS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) -I $(INCLUDES) $^ -Llibft -lft -o $(NAME)

bonus: $(OBJS_BONUS)
	$(MAKE) -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) -I $(INCLUDES) $^ -Llibft -lft -o $(NAME_BONUS)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rvf $(OBJS) $(OBJS_BONUS)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME) $(NAME_BONUS)

re: fclean all