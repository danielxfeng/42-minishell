/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 07:45:10 by Xifeng            #+#    #+#             */
/*   Updated: 2025/03/06 21:22:37 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"

// @brief if the given cmd a built-in function
//
// @param the given cmd to be matched.
// @return if the function is built-in.
bool	is_builtin_func(char *cmd)
{
	if (ms_strcmp(cmd, "cd") == 0)
		return (true);
	if (ms_strcmp(cmd, "echo") == 0)
		return (true);
	if (ms_strcmp(cmd, "pwd") == 0)
		return (true);
	if (ms_strcmp(cmd, "exit") == 0)
		return (true);
	if (ms_strcmp(cmd, "export") == 0)
		return (true);
	if (ms_strcmp(cmd, "unset") == 0)
		return (true);
	if (ms_strcmp(cmd, "env") == 0)
		return (true);
	return (false);
}

// @brief execute the built-in function.
//
// @param ast: the pointer to the ast.
// @param prop: the property of node.
// @return the status code.
int	exec_builtin_func(t_ast *ast, t_cmd_prop *prop)
{
	char	*cmd;
	int		status;

	cmd = ast->tokens[prop->start];
	if (ms_strcmp(cmd, "cd") == 0)
		status = cmd_cd(ast, prop);
	else if (ms_strcmp(cmd, "echo") == 0)
		status = cmd_echo(ast, prop);
	else if (ms_strcmp(cmd, "pwd") == 0)
		status = cmd_pwd(ast, prop);
	else if (ms_strcmp(cmd, "exit") == 0)
		status = cmd_exit(ast, prop);
	else if (ms_strcmp(cmd, "export") == 0)
		status = cmd_export(ast, prop);
	else if (ms_strcmp(cmd, "unset") == 0)
		status = cmd_unset(ast, prop);
	else if (ms_strcmp(cmd, "env") == 0)
		status = cmd_env(ast, prop);
	else
		status = EXIT_FAIL;
	return (status);
}
