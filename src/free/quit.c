/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 01:13:01 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/08 19:37:04 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Safely terminates the program.
 *
 * This function cleans up the shell environment before exiting.
 * It disables terminal key settings if applicable, frees allocated memory,
 * and then exits with the specified exit code.
 *
 * @param exit_code The exit status code returned to the system.
 *
 * @note The function checks if `keys_off` is set in the `t_termcaps` struct 
 *       and disables special key settings before quitting.
 * @todo Enable canonical mode before quitting.
 */
void	quit(int exit_code)
{
	t_termcaps	*terminal;

	terminal = &shell.termcaps;
	if (terminal -> keys_off)
		tputs(terminal -> keys_off, 1, ft_put);
	enable_canonical_mode(terminal); //TODO enable cronical
	free_shell(&shell);
	exit(exit_code);
}