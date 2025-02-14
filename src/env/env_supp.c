/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_supp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:05:24 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/14 20:03:48 by Xifeng           ###   ########.fr       */
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
bool	env_append(t_env *env, char *key, char *value)
{

	e

	keys = ft_calloc(env->size + 2, sizeof(char *));
	if (!keys)
		return (false);
	values = ft_calloc(env->size + 2, sizeof(char *));
	if (!values)
	{
		free(keys);
		return (false);
	}
	ft_memcmp(keys, env->keys, env->size);
	ft_memcmp(values, env->values, env->size);
	keys[size] = key;
	values[size] = value;
	close_kv(env->keys, env->size);
	close_kv(env->values, env->size);
	++(env->size);
	env->keys = keys;
	env->values = values;
	return (true);
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

