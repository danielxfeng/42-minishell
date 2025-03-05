/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 12:01:19 by Xifeng            #+#    #+#             */
/*   Updated: 2025/03/05 20:41:25 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include <sys/wait.h>

// @brief parses and returns the exit code from sub-processes
//
// @param status: the incoming status code.
// @return the parsed status code.
int	return_process_res(int status)
{
	int	sig;

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			ft_putstr_fd("^\\Quit: 3\n", STDERR_FILENO);
		return (sig + 128);
	}
	return (EXIT_FAIL);
}
