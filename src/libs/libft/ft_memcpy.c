/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:19:32 by Xifeng            #+#    #+#             */
/*   Updated: 2024/11/02 14:19:53 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*ptr_dest;
	const unsigned char	*ptr_src;
	const unsigned char	*stop;

	ptr_dest = dest;
	ptr_src = src;
	if (!ptr_dest && !ptr_src)
		return (dest);
	stop = src + n;
	while (ptr_src < stop)
	{
		*ptr_dest = *ptr_src;
		++ptr_src;
		++ptr_dest;
	}
	return (dest);
}
