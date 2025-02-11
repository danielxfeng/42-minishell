/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 07:45:10 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/11 10:26:11 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"

// @brief if the given cmd a built-in function
// TODO to implement.
//
// @param the given cmd to be matched.
// @return if the function is built-in.
bool	is_builtin_func(char *cmd)
{
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (true);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (true);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (true);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (true);
	return (false);
}

// @brief execute the built-in function.
// TODO to implement.
//
// @param ast: the pointer to the ast.
// @param prop: the property of node.
// @return the status code.
int	exec_builtin_func(t_ast *ast, t_cmd_prop *prop)
{
	char	*cmd;

	cmd = ast->tokens[prop->size];
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (cmd_cd(ast, prop));
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (cmd_echo(ast, prop));
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (cmd_pwd(ast, prop));
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (cmd_pwd(ast, prop));
	return (EXIT_FAIL);
}
