/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_methods.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:07:59 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/14 17:03:50 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/libft/libft.h"
#include "mini_env.h"

bool	parse_pair(char *pair[2], char *item);
void	close_kv(char **arr, int size);
bool	env_append(t_env *env, char *key, char *value);
int		find_key(t_env *env, char *key);
char	*env_strjoin(char *key, char *value);

// @brief remove the key from env.
//
// Free your item by yourself!!
// Example:
// char *key = strdup("KEY");
// if (!env_remove(env, key))
//     close_env(env);
// free(key);
//
// @param env: the pointer to env.
// @param key: the non-NULL string of key, like "KEY".
// @return false on error, returns true otherwise.
bool	env_remove(t_env *env, char *key)
{
	int	i;

	i = find_key(env, key);
	if (i == env->size)
		return (true);
	keys = ft_calloc(env->size, sizeof(char *));
	if (!keys)
		return (false);
	values = ft_calloc(env->size, sizeof(char *));
	if (!values)
	{
		free(keys);
		return (false);
	}
	ft_memcmp(keys, env->keys, i);
	ft_memcmp(values, env->values, i);
	ft_memcmp(keys + i, env->keys[i + 1], env->size - i - 1);
	ft_memcmp(values + i, env->values[i + 1], env->size - i - 1);
	close_kv(env->keys, env->size);
	close_kv(env->values, env->size);
	--(env->size);
	env->keys = keys;
	env->values = values;
	return (true);
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

	i = find_key(env, key);
	if (i == env->size)
		return (ft_strdup(""));
	return (ft_strdup(env->values[i]));
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
		envp[i] = env_strjoin(env->keys[i], env->values[i]);
		if (!envp[i])
		{
			j = 0;
			while (j < i)
			{
				free(envp[j++]);
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
// @param item: the non-NULL string of item, like "KEY=value" or "KEY=".
// @return false on error, returns true otherwise.
bool	*env_set(t_env *env, char *item)
{
	char	*pair[2];
	int		idx;

	if (!(parse_pair(pair, item))
		return (false);
	idx = find_key(env, pair[0]);
	if (idx == env->size)
		return (env_append(env, pair[0], env[pair1]));
	free(env->values[idx]);
	env->valus[idx] = pair[1];
	free(pair[0]);
	return (true);
}
