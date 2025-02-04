/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 11:13:46 by Xifeng            #+#    #+#             */
/*   Updated: 2024/11/02 14:20:10 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (src[i] != '\0')
		++i;
	count = i;
	i = 0;
	while (size > 0 && src[i] != '\0' && i < size - 1)
	{
		dst[i] = src[i];
		++i;
	}
	if (size > 0)
		dst[i] = '\0';
	return (count);
}
