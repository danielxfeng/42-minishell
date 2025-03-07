/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_travelsal_hander_supp2.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:26:14 by Xifeng            #+#    #+#             */
/*   Updated: 2025/03/06 21:29:59 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/global.h"
#include "../libs/libft/libft.h"
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

void	heredoc_expand(t_ast *ast, char **line);

// @brief handle the red sig.
//
// @param prop: the property of the node.
// @param line: the pointer to input line.
// @return error or OK.
int	handle_red_sig(t_red_prop *prop, char **line)
{
	if (g_here_doc_status == 1)
	{
		free(*line);
		*line = NULL;
		rl_done = 0;
		prop->status = 130;
		return (130);
	}
	return (EXIT_OK);
}

// @brief help to perform the possible expander and output to buf.
//
// @param ast: the pointer to the tree.
// @param line: the cmd line.
// @param fd_write: the fd to output.
// @param prop: the prop of node.
void	expand_and_write(t_ast *ast, char **line, int fd_write,
		t_red_prop *prop)
{
	if (!prop->is_quote)
		heredoc_expand(ast, line);
	write(fd_write, *line, ft_strlen(*line));
	write(fd_write, "\n", 1);
}
