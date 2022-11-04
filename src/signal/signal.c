/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signal.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mbatstra <mbatstra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/27 17:42:50 by mbatstra      #+#    #+#                 */
/*   Updated: 2022/11/04 12:06:54 by cyuzbas       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "readline/readline.h"
#include "readline/history.h"
#include "minishell.h"
#include "libft.h"
#include "exec.h"

void	signal_suppress_output(void)
{
	struct termios	s_termios;

	if (tcgetattr(0, &s_termios))
		perror("minishell: tcsetattr");
	s_termios.c_lflag &= ~ECHOCTL;
	if (tcsetattr(0, 0, &s_termios))
		perror("minishell: tcsetattr");
}

// only for running child-process
void	catch_quit(int sig)
{
	signal(sig, &catch_quit);
	g_mini.exit_code = 131;
	exit(g_mini.exit_code);
}

// handlers for SIGINT in different processes
void	catch_int(int sig)
{
	g_mini.exit_code = 1;
	signal(sig, &catch_int);
	ioctl(IN, TIOCSTI, "\n");
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	catch_int_child(int sig)
{
	g_mini.exit_code = 1;
	signal(sig, &catch_int_child);
}

void	catch_int_hrdc(int sig)
{
	(void)sig;
	g_mini.exit_code = 1;
	g_mini.interactive = 0;
	printf(">\n");
	exit(EXIT_FAILURE);
}

void	catch_parent_hrdc(int sig)
{
	g_mini.interactive = 0;
	g_mini.exit_code = 1;
	signal(sig, &catch_parent_hrdc);
}
