/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_in_here_doc.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 22:19:58 by Xifeng            #+#    #+#             */
/*   Updated: 2025/03/02 17:01:13 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"
#include <stdlib.h>

// @brief if we need to expand?
//
// @param line: the input command line.
// @param i: the index to $.
// @return if we need to expand.
static bool	need_expand(char *line, int *i)
{
	bool	multi_dollar;

	++(*i);
	if (!line[*i] || line[*i] == ' ' || line[*i] == '\t')
		return (false);
	multi_dollar = false;
	while (line[*i] == '$')
	{
		multi_dollar = true;
		++(*i);
	}
	if (multi_dollar)
		return (false);
	return (true);
}

// @brief to get the env key from input line.
//
// @param ast: the pointer to the tree.
// @param line: the input line.
// @param i: the index to the $.
// @return env key.
static char	*heredoc_expander_get_key(t_ast *ast, char *line, int i)
{
	int		end;
	char	*key;

	if (line[i] == '?')
		return (ft_strdup("?"));
	end = i;
	while (line[end] && line[end] != ' ' && line[end] != '\t'
		&& line[end] != '\'' && line[end] != '"')
		++end;
	key = ft_calloc(end - i + 1, sizeof(char));
	if (!key)
		exit_with_err(&ast, EXIT_FAIL, "minishell: malloc");
	ft_memcpy(key, line + i, end - i);
	return (key);
}

// @brief help to handle the expander.
//
// @param ast: the pointer to tree.
// @param line: the pointer to input line.
// @param i: the pointer to idx.
static void	heredoc_expander_helper(t_ast *ast, char **line, int *i)
{
	char	*key;
	char	*value;
	char	*new_line;

	if (!need_expand(*line, i))
		return ;
	key = heredoc_expander_get_key(ast, *line, *i);
	value = env_get(ast->env, key);
	if (!value)
		exit_with_err(&ast, EXIT_FAIL, "minishell: malloc");
	new_line = ft_calloc(ft_strlen(*line) + ft_strlen(value) - ft_strlen(key),
			sizeof(char));
	if (!new_line)
		exit_with_err(&ast, EXIT_FAIL, "minishell: malloc");
	ft_memcpy(new_line, *line, *i - 1);
	ft_memcpy(new_line + *i - 1, value, ft_strlen(value));
	ft_memcpy(new_line + *i + ft_strlen(value) - 1, *line + *i + ft_strlen(key),
		ft_strlen(*line) - *i - ft_strlen(key));
	free(key);
	free(value);
	free(*line);
	*line = new_line;
}

// @brief help to expand the $ in heredoc
//
// @param ast: the pointer to ast tree.
// @param line: the pointer to line.
void	heredoc_expand(t_ast *ast, char **line)
{
	int	i;

	i = 0;
	while ((*line)[i])
	{
		if ((*line)[i] == '$')
		{
			heredoc_expander_helper(ast, line, &i);
			if (i > (int)ft_strlen(*line))
				return ;
		}
		else
			++i;
	}
}
