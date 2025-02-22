/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:16:05 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/22 20:22:57 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"
#include <sys/wait.h>
#include <unistd.h>

bool		is_empty_cmd(char *cmd);
int			parse_full_cmd_and_check(t_ast *ast, t_cmd_prop *prop);
void		generate_argv(t_ast *ast, t_cmd_prop *prop);
int			return_process_res(int status);
void		close_envp(char ***envp);

// @brief perform the preprocess here.
//
// - perform build-in function
// - parsing full cmd
// - generating argv
//
// @param ast: the pointer to ast.
// @param prop: the pointer to property of node.
// @return status code.
static int	preprocess_cmd(t_ast *ast, t_cmd_prop *prop)
{
	int	status;

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
// We preprocess the node at first:
//   - errer handling, built-in function, path parsing, etc...
// Then create a new subprocess to execute the cmd.
//
// @param ast: the ast tree.
// @param ast_node: the `cmd` node.
// @return the status code.
int	cmd_handler(t_ast *ast, t_ast_node *ast_node)
{
	t_cmd_prop	*prop;
	int			status;
	char		**envp;

	debug_print_ast(ast, ast_node, "Exec Cmd.");
	prop = (t_cmd_prop *)ast_node->prop;
	if (is_empty_cmd(ast->tokens[prop->start]))
		return (return_prt_err(EXIT_CMD_ERR, NULL, ast->tokens[prop->start],
			"command not found"));
	if (is_builtin_func(ast->tokens[prop->start]))
		return (exec_builtin_func(ast, prop));
	status = preprocess_cmd(ast, prop);
	if (status != EXIT_OK)
		return (status);
	prop->pid = fork();
	if (prop->pid < 0)
		exit_with_err(&ast, EXIT_FAIL, "minishell: fork");
	envp = env_output(ast->env);
	if (!envp)
		exit_with_err(&ast, EXIT_FAIL, "minishell: malloc");
	if (prop->pid == 0)
		execve(prop->full_cmd, prop->argv, envp);
	close_envp(&envp);
	waitpid(prop->pid, &status, 0);
	return (return_process_res(status));
}
