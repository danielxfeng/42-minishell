/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 09:17:41 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/19 20:04:18 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parser.h"
#include "../libs/libft/libft.h"
#include <stdlib.h>

void            end_prev_token(t_parser *parser);
t_token_type    get_token_type(t_parser *parser);
void            skip_space(t_parser *parser);

// @brief help to handle the quotation mark.
//
// @param parser: the pointer to parser.
// @param q_handler: the pointer to handler function of single quote / double quote.
static void handle_quote_helper(t_parser *parser, void (* q_handler)(t_parser *parser))
{
    parser->token_start = parser->i;
    ++(parser->i);
    if (parser->tokens[parser->size - 1]->is_end)
    {
        append_token(parser);
        set_token(parser, parser->size - 1, get_token_type(parser));
    }
    q_handler(parser);
    append_str_to_last_token(parser, ms_substr(parser->line, parser->token_start, parser->i - parser->token_start));
    if (parser->line[parser->i] == '|' || parser->line[parser->i] == '<' || parser->line[parser->i] == '>' || parser->line[parser->i] == ' ')
    {
        end_prev_token(parser);
        skip_space(parser);
    }
    parser->token_start = parser->i;    
}

// @brief the logic of handle the double quote.
//
// @param parser: the pointer to parser.
static void double_quote_helper(t_parser *parser)
{
    while (parser->line[parser->i] && parser->line[parser->i] != '\"')
   {
        while (parser->line[parser->i] && parser->line[parser->i] != '\"' && parser->line[parser->i] != '$')
            ++(parser->i);
        if (parser->line[parser->i] == '$')
            parser_handle_expander(parser);
   }
   if (!(parser->line[parser->i]))
        exit_with_err_parser(&parser, EXIT_FAILURE, "\"");
}

// @brief the logic of handle the single quote.
//
// @param parser: the pointer to parser.
static void single_quote_helper(t_parser *parser)
{
    while (parser->line[parser->i] && parser->line[parser->i] != '\'')
        ++(parser->i);
    if (!(parser->line[parser->i]))
        exit_with_err_parser(&parser, EXIT_FAILURE, "\'");
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
int    parser_handle_double_quote(t_parser *parser)
{
    handle_quote_helper(parser, double_quote_helper);
    return (EXIT_SUCCESS);
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
int    parser_handle_single_quote(t_parser *parser)
{
    handle_quote_helper(parser, single_quote_helper);
    return (EXIT_SUCCESS);
}
