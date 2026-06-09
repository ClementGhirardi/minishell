*This project has been creaed as part of the 42 curriculum by cghirard, adbarth.*



**Description**

The `Minishell` project recreates a simplified shell written in C. It is modeled after Bash POSIX Mode, from which it replicates the following features:

COMMANDS
- Bash POSIX Mode commands are executed when entered with a relative or absolute path
- some of them (the built-ins) have been reimplemented: echo, exit, export, env, cd, pwd, unset, mostly without options
- commands return the same exit code (provided by the ‘$?’ command) as in Bash POSIX Mode

PIPES AND REDIRECTIONS
- Pipes (|) behave like Bash POSIX Mode
- The same applies to the following redirections: <, >, and >>
- The ‘<<’ redirection opens a heredoc, as does a prompt ending with a pipe
- Other redirections are not interpreted
- Error messages are the same as those in Bash POSIX Mode

SIGNALS
- Signal handling is enabled by the global variable `g_sig_status`, of type `volatile sig_atomic_t`
- It allows `Ctrl + C` and `Ctrl + D` to function as in Bash POSIX Mode
- `Ctrl + \` does nothing in interactive mode

VARIABLES
- `minishell` creates variables using the `export` command
- Prompts of the form ‘arg=abc’ are ignored
- ‘minishell’ displays a variable's value when the ‘$’ character is prepended (‘$arg’)
- Environment variables (‘PATH’, ‘HOME’, etc.) are also supported, and their values can be obtained by placing a ‘$’ in front ($HOME, etc.)

QUOTES
- Single quotes ('') and double quotes ("") are interpreted when properly closed
- They then behave as in Bash POSIX mode
- For example, $arg enclosed in single quotes ('$arg') is read as $arg, but $arg enclosed in double quotes ("$arg") is read as the value of arg

HISTORY
- ‘minishell’ maintains a history of prompts entered by the user
- You can navigate through it using the arrow keys
- The history of heredocs does not need to be managed

CONTROL OPERATORS (**bonus section only**)
- The operators ‘&&’, '||' behave as they do in Bash POSIX Mode
- ‘minishell’ also implements parenthesis for precedence
- subshells are not supported by ‘minishell’
- a prompt ending with ‘&&’ or ‘||’ opens a heredoc similar to the one opened by Bash POSIX Mode in the same case



The following functions were allowed:
readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history, printf, malloc, free, write, access, open, read,
close, fork, wait, waitpid, wait3, wait4, signal, sigaction, sigemptyset, sigaddset, kill, exit, getcwd, chdir, stat, lstat, fstat, unlink,
execve, dup, dup2, pipe, opendir, readdir, closedir, strerror, perror, isatty, ttyname, ttyslot, ioctl, getenv, tcsetattr, tcgetattr,
tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs


The readline function causes unavoidable memory leaks. To have Valgrind suppress them, a readline.supp file has been created. Valgrind takes this file into account when the --suppressions=readline.supp option is used. 


The objectives of this project are to improve students’ low-level knowledge. It allows for significant progress in C. It is also the largest project required by 42 at this stage of the curriculum and the first group project. The goal is to challenge students with a substantial project, as well as to teach them how to work in a group.




**Instructions**

‘Minishell’ uses a Makefile:
- The ‘make’ command compiles the mandatory part, which includes all the features listed above except for control operators
- The ‘make bonus’ command compiles the ‘minishell’ version with the bonus features
- The ‘make clean’ command deletes the object files (file.o)
- The ‘make fclean’ command runs ‘make clean’ and also deletes the executable (named ‘minishell’)
- The ‘make re’ command is equivalent to running ‘make fclean’ followed by ‘make’

‘Minishell’ is run by ‘./minishell’.

It is then used in the same way as any other shell.




**Resources**

Introduction to Bash POSIX Mode features:
https://w3.pppl.gov/info/bash/Bash_POSIX_Mode.html

Bash POSIX Mode manual
https://www.gnu.org/software/bash/manual/html_node/Bash-POSIX-Mode.html

Manual for the readline function:
https://man7.org/linux/man-pages/man3/readline.3.html

Manual for the signal function:
https://man7.org/linux/man-pages/man2/signal.2.html
