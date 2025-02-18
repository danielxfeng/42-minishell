/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_supp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 12:20:47 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/18 17:30:57 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parser.h"

// @brief a helper function to skip the spaces.
//
// @param parser: the pointer to parser.
void    skip_space(t_parser *parser)
{
    while (parser->line[parser->i] == ' ')
        ++(parser->i);
}

// @brief a helper function to end a token.
//
// @param parser: the pointer to parser.
void    end_prev_token(t_parser *parser)
{
    t_token *prev;
    int     len;

    prev = parser->tokens[parser->size - 1];
    if (prev->is_end)
        return  ;
    len = parser->i - parser->token_start;
    append_str_to_last_token(parser, ms_substr(parser->line, parser->token_start, len));
    prev->is_end = true;
}
