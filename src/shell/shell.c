/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 18:05:13 by Xifeng            #+#    #+#             */
/*   Updated: 2025/03/05 20:43:38 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/shell.h"
#include <errno.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>

int	g_here_doc_status = 0;

// @brief run the parser and executor.
//
// @param env: pointer to env.
// @param line: the user input.
static void	parse_and_execute(t_env *env, char *line)
{
	int			len;
	int			status;
	t_parser	*parser;
	t_ast		*ast;
	char		**tokens;

	add_history(line);
	parser = create_parser(line, env);
	env->prev_status = parse(parser);
	if (env->prev_status != EXIT_OK)
		return ;
	re_order_tokens(parser);
	tokens = output_tokens(parser);
	len = 0;
	while (tokens[len])
		++len;
	ast = build_tree(tokens, len, env, parser);
	status = ast->root->node_pre_handler(ast, ast->root);
	if (status == 130)
		env->prev_status = status;
	else
		env->prev_status = ast->root->node_handler(ast, ast->root);
	close_ast(&ast);
	return ;
}

// @brief set the prompt color by previous status.
//
// @param env: the pointer to env.
// @return: the prompt.
char	*get_prompt(t_env *env)
{
	if (env->prev_status == EXIT_OK)
		return (PROMPT_RESET);
	return (PROMPT_RED_BOLD);
}

// @brief the entrance of the program.
//
// @param envp: the env.
int	run_shell(char **envp)
{
	char	*line;
	t_env	*env;
	int		status;

	env = create_env(envp);
	sig_init();
	status = EXIT_OK;
	while (true)
	{
		line = readline("minishell ▸ ");
		if (!line && errno == ENOMEM)
			exit_with_err_shell(&env, "minishell: readline");
		else if (!line)
			return (close_and_return(&env, status));
		if (is_empty_line(line))
		{
			free(line);
			continue ;
		}
		parse_and_execute(env, line);
	}
	status = env->prev_status;
	close_env(&env);
	return (status);
}
