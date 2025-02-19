/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_methods.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 20:27:28 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/19 09:40:15 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parser.h"
#include "../libs/libft/libft.h"
#include <stdlib.h>

t_token **create_tokens(t_parser *parser, int capacity);

// @brief append a new token to parser.
// Exits the program on error.
// Will not set the properties of token.
//
// @param parser: the pointer of parser.
void    append_token(t_parser *parser)
{
    t_token **tokens;
    t_token *token;
    
    if (parser->size == parser->capacity)
    {
        tokens = create_tokens(parser, parser->capacity * 2);
        ft_memcpy(tokens, parser->tokens, parser->size * sizeof(t_token *));
        free(parser->tokens);
        parser->tokens = tokens;
        parser->capacity *= 2;
    }
    parser->tokens[parser->size] = ft_calloc(1, sizeof(t_token));
    if (!(parser->tokens[parser->size]))
        exit_with_err_parser(&parser, EXIT_FAILURE, "minishell: malloc");
    ++(parser->size);
}

// @brief set the properties of token.
//
// @param parser: the pointer to parser.
// @param type: the type of token.
// @param pipe_idx: the idx of pipe.
void    set_token(t_parser *parser, int idx, t_token_type type)
{
    t_token *token;
    
    if (type == PIPE)
    {
        ++(parser->pipe_count);
        parser->has_cmd = false;
    }
    else if (type == CMD)
        parser->has_cmd = true;
    token = parser->tokens[idx];
    token->pipe_idx = parser->pipe_count;
    token->type = type;
}

// @brief append the string to the last token in parser.
// Exits the program on error.
//
// @param parser: the pointer to parser.
// @param str: the string to append. Will free it so make sure it's on heap.
void    append_str_to_last_token(t_parser *parser, char *str)
{
    t_token *token;
    char    *joined;

    if (!str)
        exit_with_err_parser(&parser, EXIT_FAILURE, "minishell: malloc");
    token = parser->tokens[parser->size - 1];
    joined = ms_strjoin_parser(token->str, str);
    free(str);
    if (!joined)
        exit_with_err_parser(&parser, EXIT_FAILURE, "minishell: malloc");
    free(token->str);
    token->str = joined;
}

// @brief to switch the place of 2 tokens.
//
// @param: i1, i2: the indexes of tokens to switch.
void    switch_token(t_parser *parser, int i1, int i2)
{
    t_token *token;

    token = parser->tokens[i1];
    parser->tokens[i1] = parser->tokens[i2];
    parser->tokens[i2] = token;
}

// @brief output the tokens to a string array.
// And then close the parser.
//
// @param: parser: the pointer to parser.
// @return the string array of tokens.
char    **output_tokens(t_parser *parser)
{
    int     i;
    char    **tokens;

    tokens = ft_calloc(parser->size + 1, sizeof(char *));
    i = 0;
    while (i < parser->size)
    {
        tokens[i] = parser->tokens[i]->str;
        ++i;
    }
    close_parser(&parser, false);
    return (tokens);
}
