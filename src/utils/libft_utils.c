/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 18:24:21 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/09 15:49:18 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	ft_putstr(char *str)
{
	return (write(1, str, ft_strlen(str)));
}

int	ft_isspace(char c)
{
	return (c == ' ' || (9 <= c && c <= 13));
}

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 == *s2++)
		if (*s1++ == 0)
			return (0);
	return (*s1 - *--s2);
}

char	*ft_strcpy(char *dest, const char *src)
{
	char	*saved;

	saved = dest;
	while (*src)
		*dest++ = *src++;
	*dest = '\0';
	return (saved);
}

void	ft_dlstclear(t_dlist **lst, void (*del)(void*))
{
	t_dlist	*tmp;

	if (!lst)
		return ;
	while (*lst)
	{
		tmp = *lst;
		*lst = (*lst)->next;
		ft_dlstdelone(tmp, del);
	}
	*lst = 0;
}
