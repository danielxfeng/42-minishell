/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 13:54:19 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/11 12:39:42 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
* @brief Checks if it's a supported ctrl <letter> combination. All ctrl + <letter>
* have an ASCII value between 0 and 31
* @param c inputed character
* @param [type] param_value
*
* -	Ctrl-c (ie 3), ctrl-d (ie 4) and enter keypcap (ie 10) are all valid
*/
int	is_ctrl(char c)
{
	int	check;

	if (c == CTRL_C || c == CTRL_D || c == ENTER_KEYCAP)
		check = 0;
	else if ((0 <= c && c <= 31) || c == 127)
		check = 1;
	else
		check = 0;
	return (check);
}

/**
* @brief Same function as strstr but it will only look for the string to_find outside
* of quotes (single or double)
* @param str string where we are looking (aka the haystack)
* @param to_find string to find (aka the needle)
* @return address where the needle is

* -	If we find a single or double quote, we need to skip them. The next
*	character will be the one after the closing matching quote

* -	The exact next character after skip_quotes() can be a string to_find
*	so we can't use else if pattern

* - skip_quotes() can reach end of string so we need check for that
*	before incrementing otherwise we'll segfault
*/
char	*ft_strstr_quotes(char *str, char *to_find)
{
	int	i;

	i = 0;
	if (!(*to_find))
		return (str);
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
			skip_quotes(str, &i);
		if (!ft_strncmp(&str[i], to_find, ft_strlen(to_find)))
			return (&str[i]);
		if (str[i])
			i++;
	}
	return (0);
}

/**
* @brief Parses a string until the index i no longer points to a character inside
* quotes (single or double)
* @param input string to parse
* @param curr_pos index of the string. We have the reference because
*	we need to update its value for the function caller

* -	If we find a double quotes and single quotes are not open,
*	we set has_double_quotes_open to its opposite, meaning:

* - true if it was initially false (we are opening double quotes)

* - false if it was initially true (we are closing double quotes)

*	We don't care about double quotes if single quotes are open because
*	it will all be part of the token enclosed by the single quotes
* 	Exact same logic as for the double quotes
*/
void	skip_quotes(const char *input, int *curr_pos)
{
	int		has_double_quotes_open;
	int		has_single_quotes_open;

	has_double_quotes_open = 0;
	has_single_quotes_open = 0;
	while (input[*curr_pos])
	{
		if (input[*curr_pos] == '"' && has_single_quotes_open == 0)
			has_double_quotes_open = !has_double_quotes_open;
		else if (input[*curr_pos] == '\'' && has_double_quotes_open == 0)
			has_single_quotes_open = !has_single_quotes_open;
		(*curr_pos)++;
		if (!has_double_quotes_open && !has_single_quotes_open)
			break ;
	}
}

/**
* @brief Skips all spaces starting at curr_pos while incrementing *curr_pos
* @param input the unchanged line entered in stdin
* @param curr_pos the current parsing position within the input  
*/
void	skip_spaces(const char *input, int *curr_pos)
{
	while (input[*curr_pos] && ft_isspace(input[*curr_pos]))
		(*curr_pos)++;
}

/**
* @brief Checks if the char is a token delimiter
* @param: c the character tested
*/
int	is_token_delimiter(char c)
{
	int	check;

	if (ft_strchr(WHITE_SPACE, c) != 0)
		check = 1;
	else if (c == ';')
		check = 1;
	else if (c == '|')
		check = 1;
	else if (c == '&')
		check = 1;
	else if (c == '<')
		check = 1;
	else if (c == '>')
		check = 1;
	else
		check = 0;
	return (check);
}
