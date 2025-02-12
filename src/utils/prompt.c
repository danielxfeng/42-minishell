/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 19:38:56 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/12 12:46:17 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
* @brief Displays minishell prompt with colour

* - Color of the prompt will the depend on the last exit_status

* - Set back the color to white
*/
void	ft_prompt(void)
{
	if (shell.exit_status == EXIT_SUCCESS)
		ft_putstr(GREEN);
	else
		ft_putstr(RED_BOLD);
	ft_putstr("minishell → ");
	ft_putstr(RESET);
}