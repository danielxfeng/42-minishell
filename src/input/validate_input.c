/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 17:07:22 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/11 01:03:06 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
* @brief Checks if input entered is valid while displaying error messsage
* @param input the unchanged line entered in stdin

* -	Case: empty or only white space. It doesn't require an error message
* - Some testers say that the errno needs to be 2 but not very
* consistent across different OS. So we choose to set it to
* ENOEXEC (Exec format error), which value will adapt across OS
*/
int	is_input_valid(const char *input)
{
	int		check;
	char	err_msg[100];

	ft_bzero(err_msg, sizeof(err_msg)); //0
	if (*input == '\0' || ft_strisspace((char *)input))
		check = 0;
	else if (!is_input_valid_unexpected_token(input, err_msg)
		|| !is_input_valid_not_supported(input, err_msg))
	{
		check = 0;
		shell.exit_status = ENOEXEC;
		write_shell_error(err_msg);
	}
	else
		check = 1;
	return (check);
}

/**
* @brief Checks if the input has an unexpected token
* @param input the unchanged line entered in stdin
* @param err_msg error msg buffer

* - has_char_at_beginning() and has_char_at_end() trim whitespaces at
* both ends of the input before checking

* - The has_forbidden_sequence() function remove all white spaces
*  (except between quotes) before checking
*/
int	is_input_valid_unexpected_token(const char *input, char *err_msg)
{
	int		check;

	if (has_open_quote(input, err_msg)
		|| has_char_at_beginning(input, '|', err_msg)
		|| has_char_at_beginning(input, ';', err_msg)
		|| has_char_at_end(input, '|', err_msg)
		|| has_char_at_end(input, '<', err_msg)
		|| has_char_at_end(input, '>', err_msg)
		|| has_char_at_end(input, '&', err_msg)
		|| has_forbidden_sequence(input, ";;", err_msg)
		|| has_forbidden_sequence(input, "|;", err_msg)
		|| has_forbidden_sequence(input, "&;", err_msg)
		|| has_forbidden_sequence(input, ";|", err_msg)
		|| has_forbidden_sequence(input, ";&", err_msg)
		|| has_forbidden_sequence(input, ">>>", err_msg)
		|| has_spaces_between_char(input, '|', err_msg)
		|| has_spaces_between_char(input, '>', err_msg))
		check = 0;
	else
		check = 1;
	return (check);
}

/**
* @brief Checks if the input has any open quotes (single or double).

* - Considered as open quotes:
* - "
* - " '
* - " ' " '

* - Not considered as open quotes:
* - " "
* - " ' "
* - " ' ' "
* @param input the unchanged line entered in stdin
* @param err_msg error message buffer
*/
int	has_open_quote(const char *input, char *err_msg)
{
	int	check;
	int	has_dquotes_open;
	int	has_squotes_open;

	check = 1;
	has_dquotes_open = 0;
	has_squotes_open = 0;
	while (*input)
	{
		if (*input == '"' && has_squotes_open == 0)
			has_dquotes_open = !has_dquotes_open;
		else if (*input == '\'' && has_dquotes_open == 0)
			has_squotes_open = !has_squotes_open;
		input++;
	}
	if (has_dquotes_open || has_squotes_open)
	{
		check = 1;
		ft_strcpy(err_msg, "syntax error: open quotes");
	}
	else
		check = 0;
	return (check);
}

/**
* @brief Checks if there is a specific character at beginning of input
* @param input the unchanged line entered in stdin
* @param c the tested character
* @param err_msg error message buffer
*
* - We need to trim white space from the input while
* not changing the initial input
*/
int	has_char_at_beginning(const char *input, char c, char *err_msg)
{
	int		check;
	char	*cpy;

	cpy = ft_strtrim(input, WHITE_SPACE);
	if (!cpy)
		quit(EXIT_FAILURE);
	if (cpy[0] == c)
	{
		check = 1;
		ft_strcpy(err_msg, "syntax error near unexpected token `");
		ft_strncat(err_msg, &c, 1);
		ft_strncat(err_msg, "'", 2);
	}
	else
		check = 0;
	free(cpy);
	return (check);
}

/**
* @brief Checks if there is a specific character at end of input
* @param input the unchanged line entered in stdin
* @param c the tested character
* @param err_msg error message buffer
*
* - same like 
*/
int	has_char_at_end(const char *input, char c, char *err_msg)
{
	int		check;
	char	*cpy;
	int		len;

	cpy = ft_strtrim(input, WHITE_SPACE);
	if (!cpy)
		quit(EXIT_FAILURE);
	len = ft_strlen(cpy);
	if (cpy[len - 1] == c)
	{
		check = 1;
		ft_strcpy(err_msg, "syntax error near unexpected token `");
		ft_strncat(err_msg, &c, 1);
		ft_strncat(err_msg, "'", 2);
	}
	else
		check = 0;
	free(cpy);
	return (check);
}
