/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:24:55 by Xifeng            #+#    #+#             */
/*   Updated: 2025/03/04 11:00:03 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdio.h>
#include <termios.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>
#include "../libs/libft/libft.h"

static void	suppress_output(void)
{
	struct termios	termios_p;

	if (tcgetattr(0, &termios_p))
		perror("minishell: tcgetattr");
	termios_p.c_lflag &= ~ECHOCTL;
	if (tcsetattr(0, 0, &termios_p))
		perror("minishell: tcsetattr");
}

static void	reprompt(int sig)
{
	(void)sig;
	suppress_output();
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	sigquit(int sig)
{
	(void)sig;
	suppress_output();
	rl_on_new_line();
	rl_redisplay();
}

static void	interrupt(int sig)
{
	(void)sig;
	suppress_output();
	rl_on_new_line();
	rl_redisplay();
	exit(1);
}

/* SIGNAL FUNCTIONS */

void	sig_init(void)
{
	signal(SIGINT, reprompt);
	signal(SIGQUIT, sigquit);
}

void	sig_default(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	sig_ignore(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	sig_heredoc(void)
{
	signal(SIGINT, interrupt);
	signal(SIGQUIT, sigquit);
}