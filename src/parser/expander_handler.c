/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 21:08:05 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/23 12:19:19 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parser.h"
#include "../libs/libft/libft.h"
#include <stdio.h>
#include <stdlib.h>

void			skip_space(t_parser *parser);
t_token_type	get_token_type(t_parser *parser);
void			end_prev_token(t_parser *parser);
void			set_working_token(t_parser *parser);

// @brief help the expander to parse the end of a env key.
//
// @param parser: the pointer to parser.
static void	get_env_key_end(t_parser *parser)
{
	if (parser->line[parser->i] == '?')
	{
		++(parser->i);
		return ;
	}
	if (ft_isdigit(parser->line[parser->i]))
		return ;
	while ((parser->line[parser->i] >= 'a' && parser->line[parser->i] <= 'z')
		|| (parser->line[parser->i] >= 'A' && parser->line[parser->i] <= 'Z')
		|| ft_isdigit(parser->line[parser->i])
		|| parser->line[parser->i] == '_')
		++(parser->i);
}

// @brief help the expander to handle the env.
//
// @param parser: the pointer to parser.
static char	*env_helper_for_expander(t_parser *parser)
{
	char	*key;
	char	*value;

	if (parser->line[parser->i] >= '0')
		get_env_key_end(parser);
	key = ft_calloc(parser->i - parser->token_start + 1, sizeof(char));
	if (!key)
		exit_with_err_parser(&parser, EXIT_FAILURE, "minishell: malloc");
	ft_memcpy(key, parser->line + parser->token_start, parser->i
		- parser->token_start);
	value = env_get(parser->env, key);
	free(key);
	if (!value)
		exit_with_err_parser(&parser, EXIT_FAILURE, "minishell: malloc");
	return (value);
}

// @brief help the expander to get value from env.
//
// @param parser: the pointer to parser.
// @return the value of env.
static char	*env_get_helper(t_parser *parser)
{
	char	*value;

	if (parser->i - parser->token_start > 1 || parser->line[parser->i] == '\0')
	{
		value = ft_calloc(parser->i - parser->token_start + 1, sizeof(char));
		if (!value)
			exit_with_err_parser(&parser, EXIT_FAILURE, "minishell: malloc");
		ft_memcpy(value, parser->line + parser->token_start, parser->i
			- parser->token_start);
		return (value);
	}
	parser->token_start = parser->i;
	value = env_helper_for_expander(parser);
	return (value);
}

// @brief check if we actually need to expand.
// in these cases, we don't expand:
// $?, $, $ , $$aa, $|, $>, $<
//
// @param parser: the pointer to parser.
// @param is_in_double_quote: is this expander inside a double quotes block?
// @return if we need to expand.
static bool	need_expand(t_parser *parser, bool is_in_double_quote)
{
	int	i;

	i = parser->i;
	if (parser->line[i] && parser->line[i] == '?')
	{
		append_str_to_last_token(parser, ft_itoa(parser->env->prev_status));
		parser->i = i + 1;
		return (false);
	}
	while (parser->line[i] == '$')
		++i;
	if (i == parser->i && (parser->line[i] != ' ' && parser->line[i] != '\0'
			&& parser->line[i] != '<' && parser->line[i] != '>'
			&& parser->line[i] != '|' && (!is_in_double_quote
				|| parser->line[i] != '\"')))
		return (true);
	append_str_to_last_token(parser, ms_substr(parser->line, parser->i - 1, i
			- parser->i + 1));
	parser->i = i;
	return (false);
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
// @param is_in_double_quote: is this expander inside a double quotes block?
// @return status code.
int	parser_handle_expander(t_parser *parser, bool is_in_double_quote)
{
	parser->token_start = parser->i;
	++(parser->i);
	set_working_token(parser);
	if (need_expand(parser, is_in_double_quote))
		append_str_to_last_token(parser, env_get_helper(parser));
	parser->token_start = parser->i;
	return (EXIT_SUCCESS);
}
