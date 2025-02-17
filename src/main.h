/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 20:37:02 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/17 21:10:12 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# include <stdio.h>
# include <termios.h>
# include <unistd.h>
# include <termcap.h>
# include <stdlib.h>
# include <signal.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "struct.h"
# include "utils/utils.h"
# include "input/termcaps.h"
# include "free/free.h"
//# include "../lib/libft/inc/libft.h"
# include "libs/libft/libft.h"
# include "constants.h"
# include "input/input.h"
# include "parse/parse.h"


int	minishell(int ac, char **av, char **envp);

extern t_shell	shell;

#endif