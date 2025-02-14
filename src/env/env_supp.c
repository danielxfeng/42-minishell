/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:05:24 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/14 16:59:59 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/libft/libft.h"

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
bool	parse_pair(char *pair[2], char *item)
{
	int	i;

	i = 0;
	while (item[i])
	{
		if (item[i] == '=')
			break ;
		++i;
	}
	pair[0] = ft_calloc(i + 1, sizeof(char));
	if (!pair[0])
		return (false);
	pair[1] = ft_calloc(ft_strlen(item) - i, sizeof(char));
	if (!pair[1])
	{
		free(pair[0]);
		return (false);
	}
	ft_memcpy(pair[0], item, i);
	ft_memcpy(pair[1], item + i + 1, ft_strlen(item) - i - 1);
	return (true);
}

// @brief a helper function to close an array of string.
//
// @param arr: the array of string.
// @param size: the size of the array.
void	close_kv(char **arr, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (arr[i])
		{
			free(arr[i]);
			arr[i] = NULL;
		}
		++i;
	}
}

// @brief append an item to env.
//
// @param env: the pointer to env.
// @param item: the non-NULL string of item, like "KEY=value" or "KEY=".
// @return false on error, returns true otherwise.
bool	env_append(t_env *env, char *key, char *value)
{
	char	*keys;
	char	*values;

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
		if (ms_strcmp(key, env->keys[i]))
			break ;
		i++;
	}
	return (i);
}

// @brief join the key and value
//
// @param key: the key.
// @param value: the value.
// @return (key=value);
char	*env_strjoin(char *key, char *value)
{
	char	*joined;

	joined = ft_calloc((ft_strlen(key) + ft_strlen(value) + 2), sizeof(char));
	if (!joined)
		return (NULL);
	ft_memcpy(joined, key, ft_strlen(value));
	joined[ft_strlen(key)] = '=';
	ft_memcpy(joined + ft_strlen(key) + 1, cmd, ft_strlen(value));
	return (joined);
}
