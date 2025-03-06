/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 17:11:28 by Xifeng            #+#    #+#             */
/*   Updated: 2025/03/06 18:09:57 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "mini_env.h"
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
}					t_token_type;

// Represents the token.
//
// `str`: the string on heap.
// `type`: the type of token;
// `pipe_idx`: inside the n-th pipe.
// `is_end`: is this a completed token?
// `is_quote`: is the token wrapped by a quote.
typedef struct s_token
{
	char			*str;
	t_token_type	type;
	int				pipe_idx;
	bool			is_end;
	bool			is_quote;
}					t_token;

// Represents a parser.
//
// `tokens`: the container of tokens(a very simple vector).
// `env`: the env.
// `line`: the command line.
// `size`: the number of tokens in the vector.
// `capacity`: the capacity of the `tokens`.
// `pipe_count`: the number of pipes,
//  we need to assure up to one command in a pipe.
// `has_command`: if there is a command in the pipe,
//	we need to check there is up to one command in a pipe.
// `token_start`: the start index of this token.
// `i`: the iterator of the parser.
typedef struct s_parser
{
	t_token			**tokens;
	t_env			*env;
	char			*line;
	int				size;
	int				capacity;
	int				pipe_count;
	bool			has_cmd;
	int				token_start;
	int				i;
}					t_parser;

// factory

t_parser			*create_parser(char *line, t_env *env);
void				close_parser(t_parser **parser, bool is_close_str);

// methods

void				append_token(t_parser *parser);
void				append_str_to_last_token(t_parser *parser, char *str);
void				set_token(t_parser *parser, int idx, t_token_type type);
void				shift_token(t_parser *parser, int idx, int *position);
char				**output_tokens(t_parser *parser);

// handlers

int					parse(t_parser *parser);
int					parser_handle_space(t_parser *parser);
int					parser_handle_pipe(t_parser *parser);
int					parser_handle_red(t_parser *parser);
int					parser_handle_expander(t_parser *parser,
						bool is_inside_double_quote);
int					parser_handle_double_quote(t_parser *parser);
int					parser_handle_single_quote(t_parser *parser);
int					parser_handle_end(t_parser *parser);
int					parser_handle_normal(t_parser *parser);
void				re_order_tokens(t_parser *parser);

// exit fucntions

void				exit_with_err_parser(t_parser **parser, int err_code,
						char *msg);
int					return_with_err_parser(t_parser **parser, int err_code,
						char *msg);

// utils

char				*ms_strjoin_parser(char const *s1, char const *s2);
char				*ms_substr(char *str, int start, int len);
bool				is_here_doc_eof(t_parser *parser);

#endif
