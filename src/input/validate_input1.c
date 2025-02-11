/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_input1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 19:05:56 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/10 22:45:55 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
* @brief Checks if the input has a syntax error regardless of spaces between
* implemented
* @param input the unchanged line entered in stdin
* @param test tested string that has non-supported feature like "&&"
* @param err_msg error message buffer

* - ft_strstr_all returns a string with all the referenced characters
*	trimmed

* - ft_strstr_quotes behaves like strstr but it doesn't looks for
* 	the tested string between quotes (single or double). So if
*	ft_strstr_quotes returns the address of where it found test, it
*	means that the input has a syntax error
*/
int	has_forbidden_sequence(const char *input, char *test, char *err_msg)
{
	int		check;
	char	*trimmed;

	trimmed = ft_strtrim_all(input, WHITE_SPACE);
	if (!trimmed)
		quit(EXIT_FAILURE);
	if (ft_strstr_quotes(trimmed, test) != 0)
	{
		check = 1;
		ft_strcpy(err_msg, "syntax error near unexpected token `");
		ft_strncat(err_msg, test, 1);
		ft_strcat(err_msg, "'");
	}
	else
		check = 0;
	free(trimmed);
	return (check);
}

/**
* @brief Checks if a character is found twice with only spaces between them.
* Example:
* - "test | test" is valid but "test |   | test" isn't
* - "test > log.txt" and "test >> log.txt" are valid but "test >  > log" isn't
* @param input the unchanged line entered in stdin
* @param c tested char
* @param err_msg error message buffer

* -	If we find a single or double quote, skip_quotes parses through
*	the input until the quote found is closed

* - If we find a '|' and a space, we check that the next non-space
*	character is equal to '|'. If it is, then the input is invalid

* -	Both skip_quotes and skip_spaces do some parsing and they could
*	reach the end of the string so we need to check that before
*	incrementing otherwise we segfault
*/
int	has_spaces_between_char(const char *input, char c, char *err_msg)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '"' || input[i] == '\'')
			skip_quotes(input, &i);
		if (input[i] == c && input[i + 1] == ' ')
		{
			i++;
			skip_spaces((char *)input, &i);
			if (input[i] == c)
			{
				ft_strcpy(err_msg, "syntax error near unexpected token `");
				ft_strncat(err_msg, &c, 1);
				ft_strcat(err_msg, "'");
				return (1);
			}
		}
		if (input[i])
			i++;
	}
	return (0);
}

/*
* Checks if the user is trying to use a feature not implemented
* @param input the unchanged line entered in stdin
* @param error_msg error message buffer
*/
int	is_input_valid_not_supported(const char *input, char *err_msg)
{
	int		check;

	if (has_non_supported(input, "<<", err_msg)
		|| has_non_supported(input, "*", err_msg)
		|| has_non_supported(input, "\\", err_msg)
		|| has_non_supported_one(input, "&", err_msg))
		check = 0;
	else
		check = 1;
	return (check);
}

/**
* @brief Checks if the input is trying to use a specific bash feature that isn't
* implemented
* @param input the unchanged line entered in stdin
* @param test tested string that has non-supported feature like "&&"
* @param err_msg error message buffer

* -	ft_strstr_quotes behaves like strstr but it doesn't looks for
*	the tested string between quotes (single or double). So if
* 	ft_strstr_quotes returns the address of where it found test, it
*	means that the input has a non-supported bash feature
*/
int	has_non_supported(const char *input, char *test, char *err_msg)
{
	int		check;

	if (ft_strstr_quotes((char *)input, test) != 0)
	{
		check = 1;
		ft_strcpy(err_msg, "\"");
		ft_strcat(err_msg, test);
		ft_strcat(err_msg, "\" not supported");
	}
	else
		check = 0;
	return (check);
}

/**
* @brief Checks if the input is trying to use a specific bash feature that isn't
* implemented. Different from has_non_supported() because it checks if the
* the tested string is present separeted from a same character.
* For instance, "echo && test" is valid but "echo & test" isn't
* @param input the unchanged line entered in stdin
* @param test tested string that has non-supported feature like "&&"
* @param err_msg error message buffer

* -	ft_strstr_quotes behaves like strstr but it doesn't looks for
	the tested string between quotes (single or double). So if
	ft_strstr_quotes returns the address of where it found test, it
	means that the input has a non-supported bash feature
	
* - If we found one occurence of the tested string but it's repeated
*	we need to finish parsing the string to see if there are more
*	occurences
*/
int	has_non_supported_one(const char *input, char *test, char *err_msg)
{
	int		check;
	char	*tmp;

	check = 0;
	tmp = (char *)input;
	while (tmp)
	{
		tmp = ft_strstr_quotes(tmp, test);
		if (tmp != 0 && *(tmp + 1) != *test)
		{
			check = 1;
			break ;
		}
		else if (tmp != 0 && *(tmp + 1) == *test)
			tmp += 2;
	}
	if (check)
	{
		ft_strcpy(err_msg, "\"");
		ft_strcat(err_msg, test);
		ft_strcat(err_msg, "\" not supported");
	}
	return (check);
}
