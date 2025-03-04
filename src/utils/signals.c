/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 14:57:46 by Xifeng            #+#    #+#             */
/*   Updated: 2025/03/04 15:00:53 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/global.h"
#include <readline/readline.h>
#include <signal.h>

void	suppress_output(void);
void	reprompt(int sig);
void	sigquit(int sig);
void	interrupt(int sig);

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

int	stop_read_line(void)
{
	if (g_here_doc_status == 1)
	{
		rl_done = 1;
		rl_event_hook = NULL;
	}
	return (0);
}
