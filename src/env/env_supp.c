/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_supp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:05:24 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/17 17:48:28 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/libft/libft.h"
#include "mini_env.h"

bool    set_item(t_env_item *item, char *item_str);
char    *output_env_item(t_env_item *item);
char    *get_value_env_item(t_env_item *item);
bool    is_matched_env_item(t_env_item *item, char *key);
void    close_env_item(t_env_item *item);

// @brief append an item to env.
//
// @param env: the pointer to env.
// @param item: the non-NULL string of item, like "KEY=value" or "KEY=".
// @return false on error, returns true otherwise.
bool	env_append(t_env *env, char *item)
{

	e

	if (env->size == env->capacity)
	{
		env->capacity *= 2;
		items = ft_calloc(env->capacity, sizeof(t_env_item));
		if (!items)
			return (false);
		ft_memcpy(items, env->items, env->size * sizeof(t_env_item));
		free(env->items);
		env->items = items;	
	}
	++(env->size);
	return (set_item(&(env->items[env->size - 1]), item));
}

// @brief helper function to find a key from env.
//
// @param env: the pointer to env.
// @param key: the key to find.
// @return: the index of the found item. returns the size of env when not found.
int	find_key(t_env *env, char *key)
{
	int	i;

	i = 0;
	while (i < env->size)
	{
		if (is_matched_env_item(env->items[i], key))
			break ;
		i++;
	}
	return (i);
}

