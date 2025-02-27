/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:24:55 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/27 09:07:18 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdio.h>
#include <termios.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>
#include "../libs/libft/libft.h"

// /**
//  * @brief When user presses ctrl-c and ctrl-d a new line is added
//  * as opposed to the default signal interrupt behaviour
//  * @param signum signal identifier
//  */
// void	catch_sigint(int signum)
// {
// 	(void)signum;
// 	printf("\n");
// }

// /**
// * @brief prints out quit message and kills children processes
// (if we has default_signal sleep 5 | sleep 5; sleep 5
// ctrl-\ would kill minishell)
// * @param signum signal identifier
// */
// void	catch_sigquit(int signum)
// {
// 	(void)signum;
// 	printf("Quit: 3\n");
// 	kill(0, SIGCHLD);
// }

/* UTILS FOR SIGNALS */

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