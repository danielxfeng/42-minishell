/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 16:36:20 by Xifeng            #+#    #+#             */
/*   Updated: 2024/11/03 17:10:48 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa(int n)
{
	char	buf[16];
	long	ln;
	size_t	i;

	ln = n;
	if (ln == 0)
		return (ft_strdup("0"));
	ft_bzero(buf, 16);
	i = 15;
	if (ln < 0)
		ln = -ln;
	while (ln != 0)
	{
		buf[--i] = ln % 10 + '0';
		ln /= 10;
	}
	if (n < 0)
		buf[--i] = '-';
	return (ft_strdup(&buf[i]));
}
