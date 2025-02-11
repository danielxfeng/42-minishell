/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 16:25:25 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/07 16:38:13 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
* @brief Joins the variable name with a equal sign followed by the value
* @param envp pointer to environment variable string to be updated
* @param var_name Name of variable
* @param new_value Updated value string
* @return Updated string to be added to linked list
*/
char	*join_env_value(char **envp, char *var_name, char *new_value)
{
	char	*temp;
	char	*final;

	temp = ft_strjoin(var_name, "=");
	if (!temp)
		quit(EXIT_FAILURE);
	final = ft_strjoin(temp, new_value);
	if (!final)
		quit(EXIT_FAILURE);
	free(temp);
	temp = 0;
	free(*envp);
	*envp = 0;
	return (final);
}

/**
* Updates the environment variable string with a value we define
* @param var Variable to look for in the env var linked list
* @param new_value New value that will replace the current string
* @param env Environment variable string linked list
*/
void	set_env_var(char *var, char *new_value, t_list *env)
{
	int		i;
	char	*cur_env;

	while (env)
	{
		i = 0;
		cur_env = (char *)(env -> content);
		while (var[i] && cur_env[i] && (var[i] == cur_env[i]))
			i++;
		if (!var[i] && (cur_env[i] == '=' || cur_env[i] == '\0' ))
			env -> content = join_env_value(&cur_env, var, new_value);
		env = env -> next;
	}
}
