/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_factory.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 17:19:43 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/17 18:25:35 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parser.h"
#include "../libs/libft/libft.h"
#include <stdlib.h>

// @brief destructor of tokens.
//
// @param parser: the pointer to parser.
// @param tokens: the pointer to tokens.
static void    close_tokens(t_parser *parser, t_token **tokens)
{
    int i = 0;

    if (tokens && *tokens)
    {
        i = 0;
        while (i < parser->size)
            free((*tokens)[i++].str);
        free(*tokens);
        *tokens = NULL;
    }
}

// @brief destructor of parser.
//
// @param parser: the pointer to parser.
void    close_parser(t_parser **parser)
{
    int i = 0;

    if (parser && *parser)
    {
        close_tokens(*parser, &((*parser)->tokens));
        free(*parser);
        *parser = NULL;  
    }
}

// @brief constructor of tokens.
// Exits the program on error.
//
// @param parser: the pointer to parser.
// @param capacity: the capacity of the tokens.
// @return the pointer of tokens.
static t_token *create_tokens(t_parser *parser, int capacity)
{
    t_token *tokens;
    
    tokens=ft_calloc(capacity, sizeof(t_token));
    if (!tokens)
        exit_with_err_parser(&parser, EXIT_FAILURE, "minishell: malloc");
    return (tokens);
}

// @brief constructor of parser.
// Exits the program on error.
//
// @return the pointer to parser.
t_parser *create_parser()
{
    t_parser *parser;
    t_token *tokens;

    parser = ft_calloc(1, sizeof(t_parser));
    if (!parser)
        exit_with_err_parser(NULL, EXIT_FAILURE, "minishell: malloc");
    parser->capacity = INIT_CAPACITY;
    parser->tokens = create_tokens(parser, INIT_CAPACITY);
    return (parser);
}

// @brief append a new token to parser.
// Exits the program on error.
// Will not set all the properties of token.
//
// @param parser: the pointer of parser.
// @param str: the full string of the cmd.
// @param start: the start index of the token.
// @param len: the length of the token.
void    append_token(t_parser *parser, char *str, int start, int len)
{
    t_token *tokens;
    
    if (parser->size == parser->capacity)
    {
        tokens = create_tokens(parser, parser->capacity * 2);
        ft_memcpy(tokens, parser->tokens, parser->size * sizeof(t_token));
        close_tokens(parser, &(parser->tokens));
        parser->tokens = tokens;
        parser->capacity *= 2;
    }
    parser->tokens[parser->size].str = ft_calloc(len + 1, sizeof(char));
    if (!parser->tokens[parser->size].str)
        exit_with_err_parser(NULL, EXIT_FAILURE, "minishell: malloc");
    ft_memcpy(parser->tokens[parser->size].str, str + start, len);
    ++(parser->size);
}
