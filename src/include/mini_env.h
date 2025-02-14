/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 07:14:52 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/14 16:55:17 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_ENV
# define MINI_ENV

# include "utils.h"

// Represents a data structure of env.
//
// Syscall setenv is not allowed in this project,
// so that we need to maintain our own env for this project.
// We use 2 string arraies to represent a env.
//
// `keys` the array of keys of env.
// `values` the array of values of env.
// `size` the size of env.
typedef struct s_env
{
	char	**keys;
	char	**values;
	int		size;
}			t_env;

t_env	*create_env(char **envp);
t_env	*close_env(t_env **env);
char	**env_output(t_env *env);
bool	env_remove(t_env *env, char *key);
char	*env_get(t_env *env, char *key);


#endif