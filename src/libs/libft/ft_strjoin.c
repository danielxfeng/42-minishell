/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 13:34:42 by Xifeng            #+#    #+#             */
/*   Updated: 2024/11/03 19:26:55 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*p;

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
