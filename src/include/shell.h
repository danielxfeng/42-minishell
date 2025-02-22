/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 11:07:22 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/22 13:35:17 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "../include/executor.h"
# include "../include/parser.h"

# define PROMPT_RED_BOLD "\033[1;31mmsl > "
# define PROMPT_RESET "\033[0mmsl > "

void	catch_sigint(int signum);
void	catch_sigquit(int signum);

#endif
