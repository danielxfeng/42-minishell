/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 12:01:19 by Xifeng            #+#    #+#             */
/*   Updated: 2025/03/04 09:54:36 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include <sys/wait.h>
#include "include/executor.h"

// @brief parses and returns the exit code from sub-processes
//
// @param status: the incoming status code.
// @return the parsed status code.
int	return_process_res(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		int sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			ft_putstr_fd("^\\Quit: 3\n", STDERR_FILENO);
		return (sig + 128);
	}
	return (EXIT_FAIL);
}

// int	return_process_res(int status)
// {
// 	if (WIFEXITED(status))
// 		return (WEXITSTATUS(status));
// 	return (EXIT_FAIL);
// }
// int	return_process_res(int status)
// {
// 	if (WIFEXITED(status)) // Process exited normally
// 		return (WEXITSTATUS(status));
// 	else if (WIFSIGNALED(status)) // Process was terminated by a signal
// 		return (WTERMSIG(status) + 128);
// 	return (EXIT_FAIL); // Default fail case (should rarely happen)
// }