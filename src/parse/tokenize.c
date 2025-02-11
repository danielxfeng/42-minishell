/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 12:21:16 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/11 12:52:14 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
* @todo a chatgpt code :( (will edit later)
*/
char **tokenize(const char *cmd_line)
{
	char **tokens = malloc((MAX_TOKENS) * sizeof(char *));
	if (!tokens)
		quit(EXIT_FAILURE);

	int	pos = 0;
	int	token_count = 0;

	while (cmd_line[pos]) {
		while (cmd_line[pos] == ' ')  // Skip spaces
			pos++;

		if (cmd_line[pos] == '\0')
			break;

		if (token_count >= MAX_TOKENS - 1)
			break;  // Prevent overflow

        tokens[token_count] = get_token(cmd_line, &pos);
        token_count++;
    }
	
    tokens[token_count] = NULL;  // Null-terminate the array
    return tokens;
}

/**
* @brief Gets a token, which represents one argument in the cmd_line. It can
* either be a word or a string of words if quotes are used.
* Example of single valid tokens:
* - hello
* - "$TERM"'$PATH'hello1"hel'lo2"'hel"lo3'
* - hello1"hel'lo2"'hel"lo3'"$TERM"'$PATH'
* @param input the unchanged line entered in stdin
* @param  the current parsing position within the input  
* @return a str representing a single token like a program name or
*	program argument
* - We need to save the beginning of the token so that we can extract
*	the string once we have found the end of the token
*/
char	*get_token(const char *input, int *curr_pos)
{
	char	*token;
	int		saved_pos;

	saved_pos = *curr_pos;
	while (input[*curr_pos] && !is_token_delimiter(input[*curr_pos]))
	{
		if (is_quote(input[*curr_pos]))
			skip_quotes(input, curr_pos);
		else if (input[*curr_pos])
			(*curr_pos)++;
	}
	token = ft_substr(input, saved_pos, *curr_pos - saved_pos);
	if (!token)
		quit(EXIT_FAILURE);
	return (token);
}

int	is_quote(char c)
{
	int	check;

	if (c == '"' || c == '\'')
		check = 1;
	else
		check = 0;
	return (check);
}
