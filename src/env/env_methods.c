/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_methods.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:07:59 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/18 20:49:11 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/libft/libft.h"
#include "../include/mini_env.h"
#include <stdlib.h>

bool	env_append(t_env *env, char *item);
int		find_key(t_env *env, char *key);
bool    set_item(t_env_item *item, char *item_str);
char    *output_env_item(t_env_item *item);
void    close_env_item(t_env_item *item);

// @brief remove the key from env.
//
// Free your item by yourself!!
// Example:
// char *key = strdup("KEY");
// env_remove(env, key)
// free(key);
//
// @param env: the pointer to env.
// @param key: the non-NULL string of key, like "KEY".
// @return false on error, returns true otherwise.
void	env_remove(t_env *env, char *key)
{
	int	i;

	i = find_key(env, key);
	if (i == env->size)
		return ;
	close_env_item(&(env->items[i]));
	ft_memmove(env->items + i, env->items + i + 1, (env->size - i - 1) * sizeof(t_env_item));
	env->items[env->size - 1].key = NULL;
	env->items[env->size - 1].value = NULL;
	--(env->size);
}

// @brief get the value from env by given key.
//
// Free your item by yourself!!
// Example:
// char *key = strdup("KEY");
// char *value = env_get_by_key(env, key))
// free(key);
// //do something.
// free(value);
//
// @param env: the pointer to env.
// @param key: the non-NULL string of key, like "KEY".
// @return an empty string when not found, otherwise the value.
char	*env_get(t_env *env, char *key)
{
	int	i;

	if (ms_strcmp("?", key) == 0)
		return (ft_itoa(env->prev_status));
	i = find_key(env, key);
	if (i == env->size || !(env->items[i].value))
		return (ft_strdup(""));
	return (ft_strdup(env->items[i].value));
}

// @brief get the output env as envp.
//
// Don't forget to free the returned value.
//
// @param env: the pointer to env.
// @return the string array of env, NULL on error.
char	**env_output(t_env *env)
{
	int		i;
	int		j;
	char	**envp;

	envp = ft_calloc(env->size + 1, sizeof(char *));
	if (!envp)
		return (NULL);
	i = 0;
	while (i < env->size)
	{
		envp[i] = output_env_item(&(env->items[i]));
		if (!envp[i])
		{
			j = 0;
			while (j < i)
			{
				free(envp[j++]);
				free(envp);
				return (NULL);
			}
		}
		++i;
	}
	return (envp);
}

// @brief set an item to env.
//
// Free your item by yourself!!
// Example:
// char *item = strdup("KEY=value");
// if (!env_set(env, item))
//     close_env(env);
// free(item);
//
// @param env: the pointer to env.
// @param item_str: the non-NULL string of item, like "KEY=value" or "KEY=".
// @return false on error, returns true otherwise.
bool	env_set(t_env *env, char *item_str)
{
	t_env_item item;
	int		idx;

	ft_bzero(&item, sizeof(t_env_item));
	if (!set_item(&item, item_str))
		return (false);
	idx = find_key(env, item.key);
	if (idx == env->size)
	{
		close_env_item(&item);
		return (env_append(env, item_str));
	}
	if (!(item.value))
	{
		close_env_item(&item);
		return (true);
	}
	free(env->items[idx].value);
	env->items[idx].value = item.value;
	free(item.key);
	return (true);
}
