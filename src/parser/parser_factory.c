/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_factory.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 17:19:43 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/20 13:26:25 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parser.h"
#include "../libs/libft/libft.h"
#include <stdlib.h>

// @brief destructor of tokens.
//
// @param parser: the pointer to parser.
// @param tokens: the pointer to tokens.
// @param is_close_str: is also free the string in parser or not?
static void    close_tokens(t_parser *parser, t_token ***tokens, bool is_close_str)
{
    int i = 0;

    if (tokens && *tokens)
    {
        i = 0;
        while (i < parser->size)
        {
            if ((*tokens)[i])
            {
                if (is_close_str && (*tokens)[i]->str)
                {
                    free((*tokens)[i]->str);
                    (*tokens)[i]->str = NULL;
                }
                free((*tokens)[i]);
                (*tokens)[i] = NULL;
            }
            ++i;
        }
        free(*tokens);
        *tokens = NULL;
    }
}

// @brief destructor of parser.
//
// @param parser: the pointer to parser.
// @param is_close_str: is also free the string in parser or not?
// You may wanna set to true on error to free everything,
// and set to false when you still need the string like in `output_tokens()`.
void    close_parser(t_parser **parser, bool is_close_str)
{
    //int i = 0;

    if (parser && *parser)
    {
        close_tokens(*parser, &((*parser)->tokens), is_close_str);
        if ((*parser)->line)
        {
            free((*parser)->line);
            (*parser)->line = NULL;
        }
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
t_token **create_tokens(t_parser *parser, int capacity)
{
    t_token **tokens;
    
    tokens=ft_calloc(capacity, sizeof(t_token *));
    if (!tokens)
        exit_with_err_parser(&parser, EXIT_FAILURE, "minishell: malloc");
    return (tokens);
}

// @brief constructor of parser.
// Exits the program on error.
//
// @return the pointer to parser.
t_parser *create_parser(char *line, t_env *env)
{
    t_parser *parser;
    //t_token **tokens;

    parser = ft_calloc(1, sizeof(t_parser));
    if (!parser)
    {
        free(line);
        close_env(&env);
        exit_with_err_parser(NULL, EXIT_FAILURE, "minishell: malloc");
    }
    parser->line = line;
    parser->env = env;
    parser->capacity = INIT_CAPACITY;
    parser->tokens = create_tokens(parser, INIT_CAPACITY);
    return (parser);
}
