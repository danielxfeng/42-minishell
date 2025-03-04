/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 13:29:42 by Xifeng            #+#    #+#             */
/*   Updated: 2025/03/04 15:20:58 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/libft/libft.h"
#include <stdbool.h>

// @brief a special version of strjoin that supports a null s1.
char	*ms_strjoin_parser(char const *s1, char const *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*p;

	if (!s1 || !*(s1))
		return (ft_strdup(s2));
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	p = ft_calloc(len_s1 + len_s2 + 1, sizeof(char));
	if (!p)
		return (NULL);
	ft_memcpy(p, s1, len_s1);
	ft_memcpy(p + len_s1, s2, len_s2);
	p[len_s1 + len_s2] = '\0';
	return (p);
}

// @brief a substr to get the sub string.
//
// @param str: the string.
// @param start: the start index.
// @param len: the length of substring.
char	*ms_substr(char *str, int start, int len)
{
	char	*res;

	res = ft_calloc(len + 1, sizeof(char));
	if (!res)
		return (NULL);
	ft_memcpy(res, str + start, len);
	return (res);
}

// @brief check if the given line only contains space.
//
// @param line: the incoming line.
// @return if the given line contains space only.
bool	is_empty_line(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t')
			return (false);
		++i;
	}
	return (true);
}
