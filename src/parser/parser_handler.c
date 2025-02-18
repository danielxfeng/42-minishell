/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:21:27 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/18 17:30:22 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parser.h"
#include "../libs/libft/libft.h"

void    skip_space(t_parser *parser);
void    end_prev_token(t_parser *parser);

// @brief to handle the space
//
// The space is also a stop sign for the token.
// So we just end the prev token, then we skip the duplicated spaces.
// 
// @param parser: the pointer to parser.
void    parser_handle_space(t_parser *parser)
{
    if (parser->i > 0)
        end_prev_token(parser);
    skip_space(parser);
    parser->token_start = parser->i;
}

// @brief to handle the pipe
// 
// 1. Checks if there is a unclosed token, "cmd||cmd" works.
// 2. Checks if there is a legal left child,
//    which means the prev token should be `cmd`, `param`, `file`.
// 3. Creates the new token, and assigns the properties.
// 4. Updates the parser property.
// 5. Skips the spaces.
//
// @param parser: the pointer to parser.
void    parser_handle_pipe(t_parser *parser)
{
    return  ;
}

// @brief to handle the redirector
//
// 1. Checks if there is a unclosed token, "cmd<cmd" works.
// 2. Creates a new token, assign the properties.
// 3. Checks the next char, check the is_single sign.
// 4. Skip the spaces.
//
// @param parser: the pointer to parser.
void    parser_handle_red(t_parser *parser)
{
    return  ;
}

// @brief to handle the expander
// 
// 1. Tries to get a string, by doing a while loop to skip normal chars.
// 2. Checks the status of prev token, to decide if we append to prev token
//    or create a new one.
// 3. Gets from env.
// 4. If we need to create a new one, we need to check the type: 
//    `cmd`? `param`? `file`?
// 5. Skip the spaces.
//
// @param parser: the pointer to parser.
void    parser_handle_expander(t_parser *parser)
{
    return  ;
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
void    parser_handle_double_quote(t_parser *parser)
{
    return  ;
}

// @brief to handle the single_quote
//
// 1. Checks the status of prev token, to decide if we append to prev token
//    or create a new one.
// 2. If we need to create a new one, we need to check the type: 
//    `cmd`? `param`? `file`?
// 3. Tries to get a string, by doing a while loop until 
//    '\0', '\''.
// 4. Skip the '\'', and spaces.
// 
// @param parser: the pointer to parser.
void    parser_handle_single_quote(t_parser *parser)
{
    return  ;
}

// @brief to handle the end of line.
// 
// Checks the prev token
//   For 'cmd', 'file', 'param', just close it.
//   For '|', give a new line and close it.
//   For '>', it is disallowed.
//
// @param parser: the pointer to parser.
void    parser_handle_end(t_parser *parser)
{
    return  ;
}

// @brief to handle the normal char.
//
// Checks the status of prev token, to decide:
// if there is a 'cmd', 'file', 'params'.
//
// @param parser: the pointer to parser.
void    parser_handle_normal(t_parser *parser)
{
    return  ;
}
