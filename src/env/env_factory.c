/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_factory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 07:39:39 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/15 11:44:54 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/libft/libft.h"
#include "mini_env.h"
#include <stdlib.h>

bool    set_item(t_env_item *item, char *item_str);
void    close_env_item(t_env_item *item);

// @brief the destructor of env.
//
// @param env: the pointer to env.
// @return NULL.
t_env	*close_env(t_env **env)
{
    int i;

	if (env && *env)
	{
		if ((*env)->items)
        {
            i = 0;
            while (i < (*env)->size)
                close_env_item(&((*env)->items[i++]));
            free((*env)->items);
            (*env)->items = NULL;
        }
        free(*env);
        *env = NULL;
	}
	return (NULL);
}

// @brief get the size of the envp vector.
//
// @param envp: the incoming envp.
// @return the size of envp, returns 0 when envp is NULL.
static int  get_size(char **envp)
{
    int i;
    
    i = 0;
    if (!envp)
        return (0);
    while (envp[i])
		++i;
    return (i);
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

	env = ft_calloc(1, sizeof(t_env));
	if (!env)
		return (NULL);
	size = get_size(envp);
    env->size = size;
    env->capacity = size;
    if (env->capacity < INIT_CAPACITY)
        env->capacity = INIT_CAPACITY;
    env->items = ft_calloc(env->capacity, sizeof(t_env_item));
    if (!env->items)
        return (close_env(&env));
    i = 0;
    while (i < size)
    {
        if (!(set_item(&(env->items[i]), envp[i])))
            return (close_env(&env));
        ++i;
    }
	return (env);
}
