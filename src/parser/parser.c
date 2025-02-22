/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 12:17:03 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/22 12:33:02 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parser.h"
#include "../libs/libft/libft.h"
#include <stdlib.h>

// @brief Checks if the char is a token delimiter
// @param: c the character tested
// @return is the char is a delimiter.
bool	is_delimiter(char c)
{
	//int	check;

	if (c == ' ')
		return (true);
	if (c == '|')
		return (true);
	if (c == '<')
		return (true);
	if (c == '>')
		return (true);
	if (c == '$')
		return (true);
	if (c == '\0')
		return (true);
	if (c == '\"')
		return (true);
	if (c == '\'')
		return (true);
	return (false);
}

// @brief the state machine parse the command line char by char.
//
// @param parser: the pointer to parser.
// @return status code.
int	parse(t_parser *parser)
{
	char	c;
	int		status;

	while (true)
	{
		c = parser->line[parser->i];
		if (c == ' ')
			status = parser_handle_space(parser);
		else if (c == '|')
			status = parser_handle_pipe(parser);
		else if (c == '<' || c == '>')
			status = parser_handle_red(parser);
		else if (c == '$')
			status = parser_handle_expander(parser);
		else if (c == '\"')
			status = parser_handle_double_quote(parser);
		else if (c == '\'')
			status = parser_handle_single_quote(parser);
		else if (c == '\0')
			return (parser_handle_end(parser));
		else
			status = parser_handle_normal(parser);
		if (status != EXIT_SUCCESS)
			return (status);
	}
}

static void	re_order_helper(t_parser *parser, int i, int *latest)
{
	if (red_flag && (parser->tokens[i - 1]->type == CMD || parser->tokens[i
				- 1]->type == ARG))
		shift_token(parser, i - 1, &latest);
}	

// @brief re-order the tokens after parsing.
//
// The executor's requirements:
// - In one pipe block, the cmd `token` should be at first, then `arg` token.
//
// @param parser: the pointer to parser.
void	re_order_tokens(t_parser *parser)
{
	int		i;
	int		pipe_count;
	int		latest;
	bool	red_flag;

	pipe_count = 0;
	i = 0;
	latest = 0;
	red_flag = false;
	while (i++ < parser->size)
	{
		if (parser->tokens[i - 1]->pipe_idx > pipe_count)
		{
			++pipe_count;
			latest = 0;
			red_flag = false;
		}
		if (!latest && (parser->tokens[i - 1]->type == RED))
		{
			latest = i - 1;
			red_flag = true;
		}
		re_order_helper(parser, i, &latest);
	}
}
