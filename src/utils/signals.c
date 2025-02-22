/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:24:55 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/22 11:09:40 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <signal.h>

/**
 * @brief When user presses ctrl-c and ctrl-d a new line is added
 * as opposed to the default signal interrupt behaviour
 * @param signum signal identifier
 */
void	catch_sigint(int signum)
{
	(void)signum;
	ft_putstr("\n");
}

/**
* @brief prints out quit message and kills children processes
(if we has default_signal sleep 5 | sleep 5; sleep 5
ctrl-\ would kill minishell)
* @param signum signal identifier
*/
void	catch_sigquit(int signum)
{
	(void)signum;
	printf("Quit: 3\n");
	kill(0, SIGCHLD);
}
