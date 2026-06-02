/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cghirard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 23:54:39 by cghirard          #+#    #+#             */
/*   Updated: 2026/06/02 15:04:40 by cghirard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t	g_sig_status = 0;

static void	sigint_handler(int sig)
{
	(void) sig;
	if (g_sig_status == -1)
		g_sig_status = 0;
	if (g_sig_status == 1)
	{
		g_sig_status = 2;
		ft_putchar_fd('\n', 1);
		return ;
	}
	if (g_sig_status == 3)
	{
		g_sig_status = 4;
		rl_replace_line("", 0);
		rl_done = 1;
		return ;
	}
	if (g_sig_status == 5)
		return ;
	ft_putendl_fd("", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static int	exit_on_sigquit(int status, char **env)
{
	free_array(env);
	ft_putendl_fd("exit", 1);
	ft_exit(NULL, status, STDIN_FILENO, STDOUT_FILENO);
	return (status);
}

static int	event(void)
{
	return (0);
}

static int	init_var(int *status, char ***env, char **envp)
{
	*status = 0;
	*env = dup_array(envp);
	if (!(*env))
		return (*status = error_creating_env(), *status);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	rl_event_hook = event;
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	char	*input;
	char	**env;
	int		status;
	int		update_history;

	(void)ac;
	(void)av;
	if (!init_var(&status, &env, envp))
		return (status);
	while (1)
	{
		g_sig_status = -1;
		input = readline("minishell$ ");
		if (g_sig_status == 0)
			status = 130;
		if (!input)
			return (exit_on_sigquit(status, env));
		g_sig_status = 1;
		update_history = minishell(&status, &input, &env);
		if (update_history)
			add_history(input);
		free(input);
	}
	return (exit_on_sigquit(status, env));
}
