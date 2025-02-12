/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 01:39:21 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/08 19:35:41 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Frees the allocated memory for a given shell structure.
 *
 * @param shell The shell structure to be freed.
 */
void	free_shell(t_shell *shell)
{
	ft_dlstclear(&shell -> history, free);
	ft_lstclear(&shell -> dup_envp, free);
}