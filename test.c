# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <string.h>
# include <errno.h>
# include <dirent.h>

// void	function(void)
// {
// 	char *str;
// 	int status;
// 	int fd[2];
// 	pid_t pid;

// 	pipe(fd);
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		close(fd[0]);
// 		str = readline("> ");
// 		printf("str: \"%s\"\n", str);
// 		free(str);
// 		close(fd[1]);
// 		exit(0);
// 	}
// 	close(fd[1]);
// 	waitpid(pid, &status, 0);
// 	close(fd[0]);
// }




volatile sig_atomic_t	g_sig_status = 0;

int	event(void)
{
	return (0);
}

static void	sigint_handler(int sig)
{
	
	if (g_sig_status == 1)
	{
		rl_replace_line("", 0);
		rl_done = 1;
	}
	if (g_sig_status == 0)
	{
		// rl_getc_function = getc;
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	
}

int main(void)
{
	signal(SIGINT, sigint_handler);

	char *str;

	rl_event_hook = event;
	g_sig_status = 0;
	str = readline("> ");
	printf("str: \"%s\"\n", str);

	g_sig_status = 1;
	str = readline("> ");
	printf("str: \"%s\"\n", str);

	g_sig_status = 0;
	str = readline("> ");
	printf("str: \"%s\"\n", str);

	g_sig_status = 1;
	str = readline("> ");
	printf("str: \"%s\"\n", str);

	return (0);
}