/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 18:10:04 by Xifeng            #+#    #+#             */
/*   Updated: 2024/11/03 19:20:48 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	clear_helper(t_list *lst, void (*del)(void *))
{
	t_list	*next;

	next = lst->next;
	ft_lstdelone(lst, del);
	if (!next)
		return ;
	return (clear_helper(next, del));
}

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	clear_helper(*lst, del);
	*lst = NULL;
}
