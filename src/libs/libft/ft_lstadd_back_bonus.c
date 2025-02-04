/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 18:10:55 by Xifeng            #+#    #+#             */
/*   Updated: 2024/11/03 18:37:34 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	add_back_helper(t_list *lst, t_list *new)
{
	if (!lst->next)
	{
		lst->next = new;
		return ;
	}
	add_back_helper(lst->next, new);
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	add_back_helper(*lst, new);
}
