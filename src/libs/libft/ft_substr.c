/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 13:10:06 by Xifeng            #+#    #+#             */
/*   Updated: 2024/11/03 19:26:24 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	s_len;
	size_t	d_len;
	char	*p;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start > s_len)
		d_len = 0;
	else
	{
		if (s_len - start < len)
			d_len = s_len - start;
		else
			d_len = len;
	}
	p = ft_calloc((d_len + 1), sizeof(char));
	if (!p)
		return (NULL);
	ft_memcpy(p, s + start, d_len);
	p[d_len] = '\0';
	return (p);
}
