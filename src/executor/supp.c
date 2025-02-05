/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 12:01:19 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/05 12:02:31 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// @brief parses and returns the exit code from sub-processes
//
// @param status: the incoming status code.
// @return the parsed status code.
int	return_process_res(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
