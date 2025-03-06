/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_handler.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 13:40:44 by Xifeng            #+#    #+#             */
/*   Updated: 2025/03/06 10:21:59 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"
#include <stdlib.h>
#include <unistd.h>

void	close_ast_node(t_ast_node *node);
void	close_tokens(t_ast *ast);

// @brief close everything and quit the sub-process for built-in cmd.
//
// @param ast: the pointer of ast.
// @param status: the status code to return.
// @param msg: the error msg.
// @return the status code.
void	close_builtin_proc(t_ast **ast, int status, char *msg)
{
	if (msg)
		perror(msg);
	if (ast && *ast)
	{
		if ((*ast)->env)
			close_env(&((*ast)->env));
		if ((*ast)->root)
			close_ast_node((*ast)->root);
		close_tokens(*ast);
		if ((*ast)->fd_in >= 0)
			close((*ast)->fd_in);
		if ((*ast)->fd_out >= 0)
			close((*ast)->fd_out);
		free(*ast);
		*ast = NULL;
	}
	exit(status);
}

void	exit_for_cd(t_ast *ast, char **dir)
{
	free(*dir);
	*dir = NULL;
	exit_with_err(&ast, EXIT_FAIL, "minishell: malloc");
}
