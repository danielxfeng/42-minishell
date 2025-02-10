/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_close_supp.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:11:40 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/10 20:03:40 by Xifeng           ###   ########.fr       */
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
	int stdin_backup;
	int stdout_backup;
	int status;

	status = EXIT_OK;
	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (stdin_backup == -1 || stdout_backup == -1)
	{
		status = EXIT_FAIL;
		perror("minishell: dup");
	}
	if (status == EXIT_OK && 
		dup2(stdin_backup, STDIN_FILENO) || dup2(stdout_backup, STDOUT_FILENO))
	{
		status = EXIT_FAIL;
		perror("minishell: dup2");
	}
	close_fds_helper(ast);
	if (stdin_backup > 0)
		close(stdin_backup);
	if (stdout_backup > 0)
		close(stdout_backup);
	return (status);
}
