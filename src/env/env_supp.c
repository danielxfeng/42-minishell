/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_supp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:05:24 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/17 17:49:56 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/libft/libft.h"
#include "mini_env.h"

// @brief to parse the key, value from given env item.
//
// To parse the key, value from item, then save the value to pair.
// Example:
// `pair` {"KEY", "VALUE"}, `item` {"KEY=value"};
// `pair` {"KEY", ""}, `item` {"KEY="};
//
// @param pair: the container for result.
// @param item: the env item to parse.
// @return false on error, otherwise returns true.
bool    set_item(t_env_item *item, char *item_str)
{
    int	i;

	i = 0;
	while (item_str[i])
	{
		if (item_str[i] == '=')
			break ;
		++i;
	}
    if (i == ft_strlen(item_str))
        --i;
	item->key = ft_calloc(i + 1, sizeof(char));
	if (!item->key)
		return (false);
	item->value = ft_calloc(ft_strlen(item_str) - i, sizeof(char));
	if (!item->value)
	{
		free(item->key);
		return (false);
	}
	ft_memcpy(item->key, item_str, i);
	ft_memcpy(item->value, item_str + i + 1, ft_strlen(item_str) - i - 1);
	return (true);
}

// @brief free an env item.
//
// @param item: an env item to free.
void close_env_item(t_env_item *item)
{
    if (item->key)
    {
        free(item->key);
        item->key = NULL;
    }
    if (item->value)
    {
        free(item->value);
        item->value = NULL;
    }
}

// @brief append an item to env.
//
// @param env: the pointer to env.
// @param item: the non-NULL string of item, like "KEY=value" or "KEY=".
// @return false on error, returns true otherwise.
bool	env_append(t_env *env, char *item)
{
	t_env_item *items;

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
	return (set_item(&(env->items[env->size]), item));
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
		if (ms_strcmp(env->items[i].key, key))
			break ;
		i++;
	}
	return (i);
}
