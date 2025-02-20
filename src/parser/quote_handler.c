/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 09:17:41 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/22 12:25:03 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parser.h"
#include "../libs/libft/libft.h"
#include <stdlib.h>

void			end_prev_token(t_parser *parser);
t_token_type	get_token_type(t_parser *parser);
void			skip_space(t_parser *parser);
void			set_working_token(t_parser *parser);

// @brief help to handle the quotation mark.
//
// @param parser: the pointer to parser.
// @param q_handler: the pointer to handler function of 
//  single quote / double quote.
// @return (the status);
static int	handle_quote_helper(t_parser *parser,
		int (*q_handler)(t_parser *parser))
{
	int	status;

	parser->token_start = parser->i;
	++(parser->i);
	parser->token_start = parser->i;
	set_working_token(parser);
	status = q_handler(parser);
	if (status != EXIT_SUCCESS)
		return (status);
	append_str_to_last_token(parser, ms_substr(parser->line,
			parser->token_start, parser->i - parser->token_start));
	++(parser->i);
	if (parser->line[parser->i] == '|' || parser->line[parser->i] == '<'
		|| parser->line[parser->i] == '>' || parser->line[parser->i] == ' ')
	{
		end_prev_token(parser);
		skip_space(parser);
	}
	parser->token_start = parser->i;
	return (EXIT_SUCCESS);
}

// @brief the logic of handle the double quote.
//
// @param parser: the pointer to parser.
static int	double_quote_helper(t_parser *parser)
{
	while (parser->line[parser->i] && parser->line[parser->i] != '\"')
	{
		parser->token_start = parser->i;
		while (parser->line[parser->i] && parser->line[parser->i] != '\"'
			&& parser->line[parser->i] != '$')
			++(parser->i);
		if (parser->i > parser->token_start)
		{
			append_str_to_last_token(parser, ms_substr(parser->line,
					parser->token_start, parser->i - parser->token_start));
			parser->token_start = parser->i;
		}
		if (parser->line[parser->i] == '$')
		{
			parser_handle_expander(parser);
		}
	}
	if (!(parser->line[parser->i]))
		return (return_with_err_parser(&parser, 2, "\""));
	return (EXIT_SUCCESS);
}

// @brief the logic of handle the single quote.
//
// @param parser: the pointer to parser.
static int	single_quote_helper(t_parser *parser)
{
	while (parser->line[parser->i] && parser->line[parser->i] != '\'')
		++(parser->i);
	if (!(parser->line[parser->i]))
		return (return_with_err_parser(&parser, 2, "\'"));
	return (EXIT_SUCCESS);
}

// @brief to handle the double_quote
//
// 1. Checks the status of prev token, to decide if we append to prev token
//    or create a new one.
// 2. If we need to create a new one, we need to check the type:
//    `cmd`? `param`? `file`?
// 3. Tries to get a string, by doing a while loop until
//    '\0', '\"', '$'.
// 4. Call the expander handler in case we meet a '$'.
// 5. Skip the '\"', and spaces.
//
// @param parser: the pointer to parser.
// @return status code.
int	parser_handle_double_quote(t_parser *parser)
{
	return (handle_quote_helper(parser, double_quote_helper));
}

// @brief to handle the single_quote
//
// 1. Checks the status of prev token, to decide if we append to prev token
//    or create a new one.
// 2. If we need to create a new one, we need to check the type:
//    `cmd`? `param`? `file`?
// 3. Tries to get a string, by doing a while loop until
//    '\0', '\''.
//
// @param parser: the pointer to parser.
// @return status code.
int	parser_handle_single_quote(t_parser *parser)
{
	return (handle_quote_helper(parser, single_quote_helper));
}
