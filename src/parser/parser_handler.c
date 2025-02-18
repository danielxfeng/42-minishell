/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:21:27 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/18 21:05:28 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parser.h"
#include "../libs/libft/libft.h"
#include "readline/readline.h"
#include <stdio.h>

void    skip_space(t_parser *parser);
void    end_prev_token(t_parser *parser);
bool	is_delimiter(char c);

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

// @brief help pipe handler to handle the followed linebreaker.
// 
// @param parser: the pointer to parser.
static void new_input_line_for_pipe(t_parser *parser)
{
    char *input_line;
    char *new_line;
    
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
void    parser_handle_pipe(t_parser *parser)
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
    ++(parser->pipe_count);
    set_token(parser, parser->size - 1, PIPE);
    end_prev_token(parser);
    ++(parser->i);
    skip_space(parser);
    new_input_line_for_pipe(parser);
    if (parser->line[parser->i] == '|')
        return (return_with_err_parser(&parser, 2, "|"));
    parser->token_start = parser->i;
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
    end_prev_token(parser);
    skip_space(parser);
    if (parser->line[parser->i] == '\0')
        return (return_with_err_parser(&parser, 2, "`newline\'"));
    if (parser->line[parser->i] == '|' || parser->line[parser->i] == '<' || parser->line[parser->i] == '>')
        return (return_with_err_parser(&parser, 2, parser->tokens[parser->size - 1]->str));    
    parser->token_start = parser->i; 
}

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

// @brief help to calculate the token type by previous tokens.
//
// @param the pointer to parser.
// @return the token type.
t_token_type    get_token_type(t_parser *parser)
{
    if (parser->size == 1)
        return (CMD);
    if (parser->tokens[parser->size - 2] == RED)
        return (AFILE);
    if (parser->has_cmd)
        return (ARG);
    else
        return (CMD);
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
    char    *value;

    ++(parser->i);
    if (parser->tokens[parser->size - 1]->is_end)
    {
        append_token(parser);
        set_token(parser, parser->size - 1, get_token_type(parser));
    }
    append_str_to_last_token(parser, env_get_helper(parser));
    skip_space(parser);
    parser->token_start = parser->i;
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
