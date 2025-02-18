/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_supp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 12:20:47 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/18 13:05:07 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parser.h"

// @brief Checks if the char is a token delimiter
// @param: c the character tested
// @return is the char is a delimiter.
bool	is_delimiter(char c)
{
	int	check;

	if (c == ' ')
		return (true);
	if (c == '|')
        return (true);
	if (c == '<')
        return (true);
	if (c == '>')
        return (true);
    if (c == '$')
        return (true);
    if (c == '\0')
        return (true);
    if (c == '\"')
        return (true);
    if (c == '\'')
        return (true);
    return (false);
}
