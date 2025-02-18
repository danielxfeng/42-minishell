/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 21:08:05 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/18 21:28:07 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parser.h"
#include "../libs/libft/libft.h"
#include <stdio.h>

void            skip_space(t_parser *parser);
t_token_type    get_token_type(t_parser *parser);

static void get_env_key_end(t_parser *parser)
{
    if (parser->line[parser->i] == '?')
    {
        ++(parser->i);
        return ;
    }
    if (ft_isdigit(parser->line[parser->i]))
        return ;
    while ((parser->line[parser->i] >= 'a' && parser->line[parser->i] <= 'z') ||
    (parser->line[parser->i] >= 'A' && parser->line[parser->i] <= 'Z') ||
    ft_isdigit(parser->line[parser->i]) || parser->line[parser->i] == '_')
        ++(parser->i);
}

static char *env_helper_for_expander(t_parser *parser)
{
    char    *key;
    char    *value;

    if (parser->line[parser->i] >= '0' )
    get_env_key_end(parser);
    key = ft_calloc(parser->i - parser->token_start + 1);
    if (!key)
        exit_with_err_parser(&parser, EXIT_FAILURE, "minishell: malloc");
    ft_memset(key, parser->line + parser->token_start, parser->i - parser->token_start);
    value = env_get(parser->env, key);
    free(key);
    if (!value)
        exit_with_err_parser(&parser, EXIT_FAILURE, "minishell: malloc");
    return (value);
}

static char    *env_get_helper(t_parser *parser)
{
    char *value;
    
    while(parser->tokens[parser->i] == '$')
        ++(parser->i);
    if (parser->i - parser->token_start > 1 || parser->tokens[parser->i] == '\0')
    {
        value = ft_calloc(parser->i - parser->token_start + 1, sizeof(char));
        if (!value)
            exit_with_err_parser(&parser, EXIT_FAILURE, "minishell: malloc");
        ft_memset(value, parser->lien + parser->token_start, parser->i - parser->token_start);
        return (value);
    }
    return (env_get(parser->env, env_helper_for_expander(parser)));
}

// @brief to handle the expander
// 
// 1. Tries to get a string, by doing a while loop to skip normal chars.
// 2. Checks the status of prev token, to decide if we append to prev token
//    or create a new one.
// 3. Gets from env.
// 4. If we need to create a new one, we need to check the type: 
//    `cmd`? `param`? `file`?
//
// @param parser: the pointer to parser.
void    parser_handle_expander(t_parser *parser)
{
    char    *value;

    parser->token_start = parser->i;
    ++(parser->i);
    if (parser->tokens[parser->size - 1]->is_end)
    {
        append_token(parser);
        set_token(parser, parser->size - 1, get_token_type(parser));
    }
    append_str_to_last_token(parser, env_get_helper(parser));
    if (parser->i == '|' || parser->i == '<' || parser->i == '>' || || parser->i == ' ')
    {
        end_prev_token(parser);
        skip_space(parser);
    }
    parser->token_start = parser->i;
}
