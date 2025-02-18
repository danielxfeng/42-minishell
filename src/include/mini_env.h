/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 07:14:52 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/18 20:47:40 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_ENV
# define MINI_ENV

# include <stdbool.h>
# include "utils.h"

# define INIT_CAPACITY 2

// Represents a data structure of env item.
typedef struct s_env_item
{
	char	*key;
	char	*value;
}	t_env_item;

// Represents a data structure of env.
//
// Syscall setenv is not allowed in this project,
// so that we need to maintain our own env for this project.
// We use a vector to represent a env.
//
// `items` the array of keys of env.
// `values` the array of values of env.
// `size` the size of env.
// `prev_status`: the status code of previous command.
typedef struct s_env
{
	t_env_item	*items;
	int			size;
	int			capacity;
	int			prev_status;
}			t_env;

t_env	*create_env(char **envp);
t_env	*close_env(t_env **env);
void	env_remove(t_env *env, char *key);
char	*env_get(t_env *env, char *key);
bool	env_set(t_env *env, char *item_str);
char	**env_output(t_env *env);

#endif