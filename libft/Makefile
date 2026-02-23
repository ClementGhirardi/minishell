.PHONY: all clean fclean re

CC = cc

CFLAGS = -Wall -Wextra -Werror

NAME = libft.a

LIBFT_DIR = libft_srcs

FTPRINTF_DIR = ftprintf_srcs

GNL_DIR = gnl_srcs

SRCS = $(LIBFT_DIR)/ft_isalpha.c \
	$(LIBFT_DIR)/ft_isdigit.c \
	$(LIBFT_DIR)/ft_isalnum.c \
	$(LIBFT_DIR)/ft_isascii.c \
	$(LIBFT_DIR)/ft_isprint.c \
	$(LIBFT_DIR)/ft_strlen.c \
	$(LIBFT_DIR)/ft_memset.c \
	$(LIBFT_DIR)/ft_bzero.c \
	$(LIBFT_DIR)/ft_memcpy.c \
	$(LIBFT_DIR)/ft_memmove.c \
	$(LIBFT_DIR)/ft_strlcpy.c \
	$(LIBFT_DIR)/ft_strlcat.c \
	$(LIBFT_DIR)/ft_toupper.c \
	$(LIBFT_DIR)/ft_tolower.c \
	$(LIBFT_DIR)/ft_strchr.c \
	$(LIBFT_DIR)/ft_strrchr.c \
	$(LIBFT_DIR)/ft_strncmp.c \
	$(LIBFT_DIR)/ft_memchr.c \
	$(LIBFT_DIR)/ft_memcmp.c \
	$(LIBFT_DIR)/ft_strnstr.c \
	$(LIBFT_DIR)/ft_atoi.c \
	$(LIBFT_DIR)/ft_calloc.c \
	$(LIBFT_DIR)/ft_strdup.c \
	$(LIBFT_DIR)/ft_substr.c \
	$(LIBFT_DIR)/ft_strjoin.c \
	$(LIBFT_DIR)/ft_strtrim.c \
	$(LIBFT_DIR)/ft_split.c \
	$(LIBFT_DIR)/ft_itoa.c \
	$(LIBFT_DIR)/ft_strmapi.c \
	$(LIBFT_DIR)/ft_striteri.c \
	$(LIBFT_DIR)/ft_putchar_fd.c \
	$(LIBFT_DIR)/ft_putstr_fd.c \
	$(LIBFT_DIR)/ft_putendl_fd.c \
	$(LIBFT_DIR)/ft_putnbr_fd.c \
	$(LIBFT_DIR)/ft_lstnew_bonus.c \
	$(LIBFT_DIR)/ft_lstadd_front_bonus.c \
	$(LIBFT_DIR)/ft_lstsize_bonus.c \
	$(LIBFT_DIR)/ft_lstlast_bonus.c \
	$(LIBFT_DIR)/ft_lstadd_back_bonus.c \
	$(LIBFT_DIR)/ft_lstdelone_bonus.c \
	$(LIBFT_DIR)/ft_lstclear_bonus.c \
	$(LIBFT_DIR)/ft_lstiter_bonus.c \
	$(LIBFT_DIR)/ft_lstmap_bonus.c \
	$(FTPRINTF_DIR)/ft_utils_0.c \
	$(FTPRINTF_DIR)/ft_utils_1.c \
	$(FTPRINTF_DIR)/ft_utils_2.c \
	$(FTPRINTF_DIR)/ft_handle_utils_0.c \
	$(FTPRINTF_DIR)/ft_handle_utils_1.c \
	$(FTPRINTF_DIR)/ft_handle_c.c \
	$(FTPRINTF_DIR)/ft_handle_c.c \
	$(FTPRINTF_DIR)/ft_handle_d.c \
	$(FTPRINTF_DIR)/ft_handle_i.c \
	$(FTPRINTF_DIR)/ft_handle_p.c \
	$(FTPRINTF_DIR)/ft_handle_perc.c \
	$(FTPRINTF_DIR)/ft_handle_s.c \
	$(FTPRINTF_DIR)/ft_handle_u.c \
	$(FTPRINTF_DIR)/ft_handle_xlow.c \
	$(FTPRINTF_DIR)/ft_handle_xup.c \
	$(FTPRINTF_DIR)/ft_printf.c \
	$(GNL_DIR)/get_next_line_utils.c \
	$(GNL_DIR)/get_next_line.c

INCLUDES = .

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -I $(INCLUDES) -c $< -o $@

clean:
	rm -rvf $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all