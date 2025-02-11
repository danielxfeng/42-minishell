/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 00:58:46 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/08 18:28:53 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Create a copy of the envp string array
 * @param dup_envp Linked list to store copied envp
 * @param envp environment variable array of strings
 */
void	copy_env(t_list **dup_envp, char **envp)
{
	t_list	*next_env;
	int		i;
	char	*temp;

	if (!envp || !envp[0])
		quit(EXIT_FAILURE);

	i = 0;
	while (envp[i] != NULL)
	{
		temp = ft_strdup(envp[i]);
		if (!temp)
			quit(EXIT_FAILURE);
		next_env = ft_lstnew(temp);
		if (!next_env)
			quit(EXIT_FAILURE);
		ft_lstadd_back(dup_envp, next_env);
		i++;
	}
}
/**
 * @brief Clears the value of an env variable while keeping the var itself.
 * Converts "VAR=value" to "VAR=".
 * @param var_name Name of the environment variable
 */
void	clear_env_value(char *var_name)
{
	int		i;
	t_list	*envp_node;
	char	*curr_envp;

	envp_node = shell.dup_envp;
	while (envp_node)
	{
		i = 0;
		curr_envp = (char *)envp_node->content;
		while (var_name[i] && curr_envp[i] && (var_name[i] == curr_envp[i]))
			i++;
		if (!var_name[i] && curr_envp[i] == '=')
		{
			replace_string(var_name, &curr_envp);
			envp_node->content = curr_envp;
		}
		envp_node = envp_node->next;
	}
	return ;
}

/**
* @brief Gets the value stored in the environment variable linked list given a
* variable name [i.e "PATH=/usr/bin" returns "/usr/bin"]
* @param var_name name of environment variable to check
* @return String duplicate of environment variable value
*/
char	*ft_getenv(char *var_name)
{
	int		i;
	t_list	*envp;
	char	*curr_envp;
	char	*value;

	envp = shell.dup_envp;
	while (envp)
	{
		i = 0;
		curr_envp = (char *)envp->content;
		while (var_name[i] && curr_envp[i] && (var_name[i] == curr_envp[i]))
			i++;
		if (!var_name[i] && (curr_envp[i] == '=' || curr_envp[i] == '\0'))
		{
			value = ft_strdup(&(curr_envp[i + 1]));
			if (!value)
				quit(EXIT_FAILURE);
			return (value);
		}
		envp = envp->next;
	}
	return (NULL);
}
