/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 18:08:20 by Xifeng            #+#    #+#             */
/*   Updated: 2024/11/03 19:10:39 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "stdlib.h"

int	map_helper(t_list *node, t_list *lst, void *(*f)(void *))
{
	t_list	*p;

	if (!lst)
		return (1);
	p = ft_lstnew(f(lst->content));
	if (!p)
		return (0);
	node->next = p;
	return (map_helper(p, lst->next, f));
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*p;

	if (!lst || !f)
		return (NULL);
	p = ft_lstnew(f(lst->content));
	if (!p)
		return (NULL);
	if (!map_helper(p, lst->next, f))
	{
		ft_lstclear(&p, del);
		return (NULL);
	}
	return (p);
}
