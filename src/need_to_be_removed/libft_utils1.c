/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:51:59 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/10 17:12:23 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	ft_dlstdelone(t_dlist *lst, void (*del)(void*))
{
	del(lst->data);
	free(lst);
}

t_dlist	*ft_dlstnew(void *data)
{
	t_dlist	*new_node;

	new_node = malloc(sizeof(t_dlist));
	if (!new_node)
		return (0);
	new_node->data = data;
	new_node->next = 0;
	new_node->prev = 0;
	return (new_node);
}

void	ft_dlstadd_front(t_dlist **lst, t_dlist *new)
{
	new->next = *lst;
	new->prev = 0;
	if (*lst)
		(*lst)->prev = new;
	*lst = new;
}

int	ft_strisspace(char *str)
{
	int	check;

	if (!str || !(*str))
		return (0);
	check = 1;
	while (*str)
	{
		if (!ft_isspace(*str))
		{
			check = 0;
			break ;
		}
		str++;
	}
	return (check);
}

char	*ft_strncat(char *dest, char *src, unsigned int nb)
{
	char	*sav_d;

	sav_d = dest;
	while (*dest)
		dest++;
	while (nb && *src)
	{
		*dest++ = *src++;
		nb--;
	}
	*dest = '\0';
	return (sav_d);
}
