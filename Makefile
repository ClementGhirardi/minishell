.PHONY: all clean fclean re

CC = cc

CFLAGS = -Wall -Wextra -Werror

NAME = minishell

NAME_BONUS = 

SRCS_DIR = srcs

SRCS_BONUS_DIR = srcs/srcs_bonus

SRCS = $(SRCS_DIR)/ft_strjoin_and_free.c \
	$(SRCS_DIR)/here_doc_word.c \
	$(SRCS_DIR)/token.c \
	$(SRCS_DIR)/lexer.c \
	$(SRCS_DIR)/ast_utils.c \
	$(SRCS_DIR)/ast.c \
	$(SRCS_DIR)/ft_gethole_fd.c \
	$(SRCS_DIR)/parser.c \
	$(SRCS_DIR)/ft_getenv.c \
 	$(SRCS_DIR)/expander.c \
	$(SRCS_DIR)/idx_to_next_line.c \
	$(SRCS_DIR)/get_buffer.c \
	$(SRCS_DIR)/here_doc.c \
	$(SRCS_DIR)/get_path.c \
	$(SRCS_DIR)/get_status.c \
	$(SRCS_DIR)/array.c \
	$(SRCS_DIR)/ft_setenv.c \
	$(SRCS_DIR)/ft_echo.c \
	$(SRCS_DIR)/ft_cd.c \
	$(SRCS_DIR)/ft_pwd.c \
	$(SRCS_DIR)/ft_export.c \
	$(SRCS_DIR)/ft_unset.c \
	$(SRCS_DIR)/ft_env.c \
	$(SRCS_DIR)/ft_exit.c \
	$(SRCS_DIR)/builtin.c \
	$(SRCS_DIR)/executor.c \
	$(SRCS_DIR)/syntax_analyzer.c \
	$(SRCS_DIR)/browse_ast_for_heredoc.c \
	$(SRCS_DIR)/lexer_handle_other_lines.c \
	$(SRCS_DIR)/here_doc_utils.c \
	$(SRCS_DIR)/data.c \
	$(SRCS_DIR)/parser_cmd_redir.c \
	$(SRCS_DIR)/split_bracket.c \
	$(SRCS_DIR)/space_bracket.c \
	$(SRCS_DIR)/executor_helper.c \
	$(SRCS_DIR)/lexer_word_bonus.c \
	$(SRCS_DIR)/syntax_brackets.c \
	$(SRCS_DIR)/errors_beginning.c \
	$(SRCS_DIR)/errors_cmd.c \
	$(SRCS_DIR)/errors_exit.c \
	$(SRCS_DIR)/errors.c \
	$(SRCS_DIR)/handle_signals.c \
	$(SRCS_DIR)/expander_remove_empty.c \
	$(SRCS_DIR)/expander_extract.c \
	$(SRCS_DIR)/ft_realloc.c \
	$(SRCS_DIR)/here_doc_functions.c \
	$(SRCS_DIR)/lexer_handle_end_with_pipe.c \

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
	$(CC) $(CFLAGS) -I $(INCLUDES)  $^ -Llibft -lft -lreadline -o $(NAME)

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