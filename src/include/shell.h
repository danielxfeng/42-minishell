/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 11:07:22 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/27 11:42:34 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "../include/executor.h"
# include "../include/parser.h"

# define PROMPT_RED_BOLD "\033[1;31mminishell> "
# define PROMPT_RESET "\033[0mminishell > "

void	exit_with_err_shell(t_env **env, char *msg);
int		close_and_return(t_env **env, int status);
bool	is_empty_line(char *line);

#endif
