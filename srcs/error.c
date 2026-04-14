#include "../includes/minishell.h"

void	*syntax_error(char *c)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("syntax error near unexpected token `", 2);
	ft_putstr_fd(c, 2);
	ft_putendl_fd("'", 2);
	return (NULL);
}
