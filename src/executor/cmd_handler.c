/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:16:05 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/08 15:58:04 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"
#include <sys/wait.h>
#include <unistd.h>

bool		is_empty_cmd(char *cmd);
int			parse_full_cmd_and_check(t_ast *ast, t_cmd_prop *prop);
void		generate_argv(t_ast *ast, t_cmd_prop *prop);

// @brief to check 1 empty cmd, 2 build-in function, 3 parse full cmd,
// 4 generate argv.
//
// @param ast: the pointer to ast.
// @param prop: the pointer to property of node.
// @return status code.
static int preprocess_cmd(t_ast *ast, t_cmd_prop *prop)
{
    char *cmd;
    int status;
    
    cmd = ast->tokens[prop->start];
	if (is_empty_cmd(ast->tokens[prop->start]))
		return (return_prt_err(EXIT_CMD_ERR, NULL, cmd, "command not found"));
	if (is_builtin_func(ast->tokens[prop->start]))
		return (exec_builtin_func(ast, prop));
	status = parse_full_cmd_and_check(ast, prop);
	if (status != 0)
		return (status);
	generate_argv(ast, prop);
    return (EXIT_OK);
}

// @ brief to execute the `cmd` node.
//
// The command node is a leaf node.
//
// @param ast: the ast tree.
// @param ast_node: the `cmd` node.
// @return the status code.
int	cmd_handler(t_ast *ast, t_ast_node *ast_node)
{
	t_cmd_prop	*prop;
	int			status;

	debug_print_ast(ast, ast_node, "");
	prop = (t_cmd_prop *)ast_node->prop;
	status = preprocess_cmd(ast, prop);
	if (status != EXIT_OK)
		return (status);
	prop->pid = fork();
	if (prop->pid < 0)
		exit_with_err(&ast, EXIT_FAIL, "minishell: fork");
	if (prop->pid == 0 && execve(prop->full_cmd, prop->argv, NULL) < 0)
		return (return_prt_err(EXIT_FAIL, "minishell", ast->tokens[prop->start],
			NULL));
	waitpid(prop->pid, &status, 0);
	return (return_process_res(status));
}
