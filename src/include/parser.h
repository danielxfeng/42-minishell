/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 17:11:28 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/18 13:49:08 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdbool.h>

# define INIT_CAPACITY 2

// Represents the type of tokens.
typedef enum e_token_type
{
    SPACE,
    PIPE,
    CMD,
    RED,
    AFILE,
    ARG,
    DOLLAR,
    DQUOTE,
    SQUOTE,
} t_token_type;

// Represents the token.
//
// `str`: the string on heap.
// `type`: the type of token;
// `pipe_idx`: inside the n-th pipe.
typedef struct s_token
{
    char *str;
    t_token_type type;
    int pipe_idx;
} t_token;

// Represents a parser.
//
// `tokens`: the container of tokens(a very simple vector).
// `size`: the number of tokens in the vector.
// `capacity`: the capacity of the `tokens`.
// `pipe_count`: the number of pipes, we need to assure up to one command in a pipe.
// `has_command`: if there is a command in the pipe, we need to check there is up to one command in a pipe.
// `token_start`: the start index of this token.
// `i`: the iterator of the parser.
typedef struct s_parser
{
    t_token **tokens;
    int size;
    int capacity;
    int pipe_count;
    bool has_cmd;
    int token_start;
    int i;
} t_parser;


t_parser    *create_parser();
void        close_parser(t_parser **parser, bool is_close_str);

void        append_token(t_parser *parser);
void        append_str_to_last_token(t_parser *parser, char *str);
void        set_token(t_parser *parser, int idx, t_token_type type, int pipe_idx);
void        switch_token(t_parser *parser, int i1, int i2);
char        **output_tokens(t_parser *parser);

void	    exit_with_err_parser(t_parser **parser, int err_code, char *msg);
void	    return_with_err_parser(t_parser **parser, int err_code, char *msg);

char	    *ms_strjoin_parser(char const *s1, char const *s2);
char        *ms_substr(char *str, int start, int len);

#endif
