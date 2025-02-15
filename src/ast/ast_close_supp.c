/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_close_supp.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:11:40 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/15 15:33:51 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include <stdlib.h>
#include <unistd.h>

// @brief destructor of tokens.
void	close_tokens(t_ast *ast)
{
	int	i;

	if (!(ast->tokens) || ast->tk_size <= 0)
		return ;
	i = 0;
	while (i < ast->tk_size)
	{
		free(ast->tokens[i]);
		(ast->tokens)[i++] = NULL;
	}
	free(ast->tokens);
	ast->tokens = NULL;
}

// @brief helper function to close_fds.
//
// @param ast: pointer to ast tree.
static void	close_fds_helper(t_ast *ast)
{
	if (ast->fd_in > 0)
		close(ast->fd_in);
	if (ast->fd_out > 0)
		close(ast->fd_out);
}

// @brief help to close the fds.
//
// @param ast: pointer to ast tree.
// @return status code.
int	close_fds(t_ast *ast)
{
	int	backup_fds[2];
	int	status;

	status = EXIT_OK;
	backup_fds[0] = dup(STDIN_FILENO);
	backup_fds[1] = dup(STDOUT_FILENO);
	if (backup_fds[0] == -1 || backup_fds[1] == -1)
	{
		status = EXIT_FAIL;
		perror("minishell: dup");
	}
	if (status == EXIT_OK && (dup2(backup_fds[0], STDIN_FILENO) == -1
		|| dup2(backup_fds[1], STDOUT_FILENO) == -1))
	{
		status = EXIT_FAIL;
		perror("minishell: dup2");
	}
	close_fds_helper(ast);
	if (backup_fds[0] > 0)
		close(backup_fds[0]);
	if (backup_fds[1] > 0)
		close(backup_fds[1]);
	return (status);
}
