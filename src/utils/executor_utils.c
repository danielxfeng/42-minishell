/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 19:27:57 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/22 13:23:33 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"

// @brief handle the string cmp.
//
// @param s1: the first string.
// @param s2: the 2nd string.
// @return 0 for equal.
int	ms_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		++i;
	}
	return (s1[i] - s2[i]);
}

// @brief a custom atoi function.
//
// @param n: the number in string format.
// @param nb: the container of result number.
// @return if it's a valid number.
bool	ms_atoi(char *n, int *nb)
{
	bool	is_negative;

	is_negative = false;
	while (*n && *n == ' ')
		++n;
	if (*n == '+' || *n == '-')
	{
		if (*n == '-')
			is_negative = true;
		++n;
	}
	*nb = 0;
	while (*n && *n >= '0' && *n <= '9')
	{
		*nb = *nb * 10 + *n - '0';
		++n;
	}
	if (is_negative)
		*nb = -(*nb);
	if (*n == '\0')
		return (true);
	return (false);
}
