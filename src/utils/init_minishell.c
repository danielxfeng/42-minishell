/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shellshell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 23:55:28 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/06 01:12:13 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Initialize the minishell environment
 * 
 * - Checks if STDERR is a terminal.
 * 
 * - Clears the `shell` struct using `ft_bzero`.
 * 
 * - copy `envp` for further manipulation
 * 
 * - Ensures the shell runs in an interactive terminal, sets up environment 
 * variables, initializes terminal settings, and configures signal handling.
 * 
 * @param shell main struct of the program
 * @param envp environment pointer to be duplicated
 * 
 */
void	init_minishell(t_shell *shell, char **envp)
{
	if (!isatty(STDERR_FILENO))
		quit(EXIT_FAILURE);
	ft_bzero(shell, sizeof(t_shell));
	copy_env(&shell -> dup_envp, envp);
	reset_oldpwd();
	increment_shlvl();
	init_termcaps(&shell -> termcaps);
	signal(SIGINT, catch_sigint);
	signal(SIGQUIT, catch_sigquit);
}
