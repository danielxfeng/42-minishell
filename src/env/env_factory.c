/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_factory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 07:39:39 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/14 17:04:55 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/libft/libft.h"
#include "mini_env.h"

bool		parse_pair(char *pair[2], char *item);
void		close_kv(char **arr, int size);

// @brief the destructor of env.
//
// @param env: the pointer to env.
// @return NULL.
t_env	*close_env(t_env **env)
{
	if (env && *env)
	{
		if ((*env)->keys)
		{
			close_kv((*env)->keys, (*env)->size);
			(*env)->keys = NULL;
		}
		if ((*env)->values)
		{
			close_kv((*env)->values, (*env)->size);
			(*env)->values = NULL;
		}
		free(*env);
		*env = NULL;
	}
	return (NULL);
}

// @brief a helper function to assign the value of env.
//
// @param env: the pointer to env.
// @param envp: the raw envp from program argument.
// @return false on error, otherwise returns true.
static bool	set_kv(t_env *env, char **envp)
{
	int		i;
	char	*empty_envp[1];

	i = 0;
	while (i < env->size)
	{
		if (!parse_pair(envp[i], pair))
			return (false);
		env->keys[i] = pair[0];
		env->values[i] = pair[1];
		++i;
	}
	return (true);
}

// @brief create an instance of env.
//
// @param envp: the env come from the system env.
// @return a new instance of env, returns an empty instance when empty,
// and returns NULL on error.
t_env	*create_env(char **envp)
{
	int		i;
	int		size;
	t_env	*env;
	char	*pair[2];

	env = ft_calloc(1, sizeof(t_env));
	if (!env)
		return (NULL);
	i = 0;
	while (envp[i++])
		++size;
	env->keys = ft_calloc(size + 1, sizeof(char *));
	if (!env->keys)
		return (close_env(&env));
	env->values = ft_calloc(size + 1, sizeof(char *));
	if (!env->values)
		return (close_env(&env));
	if (!set_kv(env, envp))
		return (close_env(&env));
	return (env);
}
