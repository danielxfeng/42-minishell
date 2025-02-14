/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_factory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 07:39:39 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/14 08:14:14 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_env.h"
#include "../libs/libft/libft.h"

// @brief a helper function to close an array of string.
//
// @param arr: the array of string.
// @param size: the size of the array.
static void close_kv(char **arr, int size)
{
    int i;

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

// @brief the destructor of env.
//
// @param env: the pointer to env.
// @return NULL.
t_env    *close_env(t_env **env)
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
static bool parse_pair(char *pair[2], char *item)
{
    int i;

    i = 0;
    while (item[i])
    {
        if (item[i] == '=')
            break;
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

// @brief a helper function to assign the value of env.
//
// @param env: the pointer to env.
// @param envp: the raw envp from program argument.
// @return false on error, otherwise returns true.
static bool set_kv(t_env *env, char **envp)
{
    int i;
    
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
t_env *create_env(char **envp)
{
    int     i;
    int     size;
    t_env   *env;
    char    *pair[2];

    env = ft_calloc(1, sizeof(t_env));
    if (!env)
        return (NULL);
    i = 0;
    while (envp[i++])
        ++size;
    env->keys = ft_calloc(size, sizeof(char *));
    if (!env->keys)
        return (close_env(&env));
    env->values = ft_calloc(size, sizeof(char *));
    if (!env->values)
        return (close_env(&env));
    if (!set_kv(env, envp))
        return (close_env(&env));
    return (env);
}
