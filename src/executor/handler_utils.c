/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 11:55:38 by Xifeng            #+#    #+#             */
/*   Updated: 2024/12/11 11:55:58 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipe_x.h"

int	return_process_res(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_FAILURE);
}
