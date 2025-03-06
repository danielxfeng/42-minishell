/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 11:07:22 by Xifeng            #+#    #+#             */
/*   Updated: 2025/03/06 21:34:49 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "../include/executor.h"
# include "../include/parser.h"

void	exit_with_err_shell(t_env **env, char *msg);
int		close_and_return(t_env **env, int status);
bool	is_empty_line(char *line);

//extern int g_heredoc_interrupted = 0;

#endif
