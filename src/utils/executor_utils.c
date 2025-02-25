/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 19:27:57 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/25 20:23:56 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"
#include <stdint.h>

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

static bool	is_overflow(char *n, int64_t lb, bool is_negative)
{
	if (lb > INT64_MAX / 10 
		|| (lb == INT64_MAX / 10 
			&& ((!is_negative && (*n - '0') > (INT64_MAX % 10)) 
			|| (is_negative && (*n - '0') > (INT64_MAX % 10) + 1))))
		return (true);
	return (false);
}

// @brief a custom atoi function.
//
// @param n: the number in string format.
// @param nb: the container of result number.
// @return if it's a valid number.
bool	ms_atoi(char *n, int *nb)
{
	bool	is_negative;
	int64_t	lb;

	is_negative = false;
	while (*n && *n == ' ')
		++n;
	if (*n == '+' || *n == '-')
	{
		if (*n == '-')
			is_negative = true;
		++n;
	}
	lb = 0;
	while (*n && *n >= '0' && *n <= '9')
	{
		if (is_overflow(n, lb, is_negative))
			return (false);
		lb = lb * 10 + *(n++) - '0';
	}
	if (is_negative)
		lb = -lb;
	*nb = (int)lb;
	if (*n == '\0')
		return (true);
	return (false);
}
