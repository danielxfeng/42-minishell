/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_supp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 12:20:47 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/22 12:38:37 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parser.h"
#include "../libs/libft/libft.h"
#include <readline/readline.h>
#include <stdlib.h>

// @brief a helper function to skip the spaces.
//
// @param parser: the pointer to parser.
void	skip_space(t_parser *parser)
{
	while (parser->line[parser->i] == ' ')
		++(parser->i);
}

// @brief a helper function to end a token.
//
// @param parser: the pointer to parser.
void	end_prev_token(t_parser *parser)
{
	t_token	*prev;
	int		len;

	if (parser->size == 0)
		return ;
	prev = parser->tokens[parser->size - 1];
	if (prev->is_end)
		return ;
	len = parser->i - parser->token_start;
	append_str_to_last_token(parser, ms_substr(parser->line,
			parser->token_start, len));
	prev->is_end = true;
}

// @brief help to calculate the token type by previous tokens.
//
// @param the pointer to parser.
// @return the token type.
t_token_type	get_token_type(t_parser *parser)
{
	if (parser->size == 1)
		return (CMD);
	if (parser->tokens[parser->size - 2]->type == RED)
		return (AFILE);
	if (parser->has_cmd)
		return (ARG);
	else
		return (CMD);
}

// @brief help pipe handler to handle the followed linebreaker.
//
// @param parser: the pointer to parser.
void	new_input_line_for_pipe(t_parser *parser)
{
	char	*input_line;
	char	*new_line;

	if (parser->line[parser->i] == '\0')
	{
		input_line = readline("> ");
		if (!input_line)
			exit_with_err_parser(&parser, EXIT_FAILURE, "minishell: malloc");
		new_line = ft_strjoin(parser->line, input_line);
		free(input_line);
		if (!new_line)
			exit_with_err_parser(&parser, EXIT_FAILURE, "minishell: malloc");
		free(parser->line);
		parser->line = new_line;
		skip_space(parser);
	}
}

// @brief help the quote, normal, expander token 
// to decide the current working token.
// if it's neccessary to create a new token, create and set it.
//
// @param parser: the pointer to parser.
void	set_working_token(t_parser *parser)
{
	if (parser->size == 0 || parser->tokens[parser->size - 1]->is_end)
	{
		append_token(parser);
		set_token(parser, parser->size - 1, get_token_type(parser));
	}
}

// @brief help the quote, normal,
	expander token to decide the current working token.
// if it's neccessary to create a new token, create and set it.
//
// @param parser: the pointer to parser.
void	set_working_token(t_parser *parser)
{
	if (parser->size == 0 || parser->tokens[parser->size - 1]->is_end)
	{
		append_token(parser);
		set_token(parser, parser->size - 1, get_token_type(parser));
	}
}
