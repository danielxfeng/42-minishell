/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:21:27 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/22 12:33:43 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parser.h"
#include "../libs/libft/libft.h"
#include "readline/readline.h"
#include <stdio.h>
#include <stdlib.h>

void			skip_space(t_parser *parser);
void			end_prev_token(t_parser *parser);
bool			is_delimiter(char c);
t_token_type	get_token_type(t_parser *parser);
void			new_input_line_for_pipe(t_parser *parser);
void			set_working_token(t_parser *parser);

// @brief to handle the space.
//
// The space is also a stop sign for the token.
// So we just end the prev token, then we skip the duplicated spaces.
//
// @param parser: the pointer to parser.
// @return status code.
int    parser_handle_space(t_parser *parser)
{
    if (parser->i > 0)
        end_prev_token(parser);
    skip_space(parser);
    parser->token_start = parser->i;
    return (EXIT_SUCCESS);
}

// @brief to handle the pipe
//
// 1. Checks if there is a unclosed token, "cmd||cmd" works.
// 2. Checks if there is a legal left child,
//    which means the prev token should be `cmd`, `param`, `file`.
// 3. Creates the new token, and assigns the properties.
// 4. Updates the parser property.
// 5. Skips the spaces.
// 6. Check the right side,
//
// @param parser: the pointer to parser.
// @return status code;
int    parser_handle_pipe(t_parser *parser)
{
    t_token_type prev_type;
    
    if (parser->size == 0)
        return (return_with_err_parser(&parser, 2, "|"));
    end_prev_token(parser);
    prev_type = parser->tokens[parser->size - 1]->type;
    if (prev_type != CMD && prev_type != AFILE && prev_type != ARG)
        return (return_with_err_parser(&parser, 2, "|"));
    append_token(parser);
    append_str_to_last_token(parser, ft_strdup("|"));
    set_token(parser, parser->size - 1, PIPE);
    end_prev_token(parser);
    ++(parser->i);
    skip_space(parser);
    new_input_line_for_pipe(parser);
    if (parser->line[parser->i] == '|')
        return (return_with_err_parser(&parser, 2, "|"));
    parser->token_start = parser->i;
    return (EXIT_SUCCESS);
}

// @brief to handle the redirector
//
// 1. Checks if there is a unclosed token, "cmd<cmd" works.
// 2. Creates a new token, assign the properties.
// 3. Checks the next char, check the is_single sign.
// 4. Skip the spaces.
//
// @param parser: the pointer to parser.
// @return the status code.
int    parser_handle_red(t_parser *parser)
{
    end_prev_token(parser);
    append_token(parser);
    set_token(parser, parser->size - 1, RED);
    if (parser->line[parser->i + 1] == '\0')
        return (return_with_err_parser(&parser, 2, "`newline\'"));
    if (parser->line[parser->i + 1] == parser->line[parser->i])
    {
        append_str_to_last_token(parser, ms_substr(parser->line, parser->i, 2));
        parser->i += 2;
    }
    else
    {
        append_str_to_last_token(parser, ms_substr(parser->line, parser->i, 1));
        ++(parser->i);
    }
    parser->token_start = parser->i;
    end_prev_token(parser);
    skip_space(parser);
    if (parser->line[parser->i] == '\0')
        return (return_with_err_parser(&parser, 2, "`newline\'"));
    if (parser->line[parser->i] == '|' || parser->line[parser->i] == '<' || parser->line[parser->i] == '>')
        return (return_with_err_parser(&parser, 2, parser->tokens[parser->size - 1]->str));    
    parser->token_start = parser->i; 
    return (EXIT_SUCCESS);
}

// @brief to handle the end of line.
//
// Checks the prev token
//   For 'cmd', 'file', 'param', just close it.
//   For '|', give a new line and close it.
//   For '>', it is disallowed.
//
// @param parser: the pointer to parser.
// @return status code.
int    parser_handle_end(t_parser *parser)
{
    end_prev_token(parser);
    return (EXIT_SUCCESS);
}

// @brief to handle the normal char.
//
// Checks the status of prev token, to decide:
// if there is a 'cmd', 'file', 'params'.
//
// @param parser: the pointer to parser.
// @return status code.
int    parser_handle_normal(t_parser *parser)
{
    end_prev_token(parser);
    append_token(parser);
    set_token(parser, parser->size - 1, get_token_type(parser));
    while (!(is_delimiter(parser->line[parser->i])))
        ++(parser->i);
    append_str_to_last_token(parser, ms_substr(parser->line, parser->token_start, parser-> i - parser->token_start));
    if (parser->i == '|' || parser->i == '<' || parser->i == '>' || parser->i == ' ')
    {
        end_prev_token(parser);
        skip_space(parser);
    }
    parser->token_start = parser->i;
    return (EXIT_SUCCESS);
}
