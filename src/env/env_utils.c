/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 01:35:37 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/08 18:29:15 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
* @brief Ensures that the OLDPWD environment variable exists but has no value.
* If OLDPWD is missing, it is created as an empty variable.
* If OLDPWD exists, its value is cleared.
*
* This prevents errors in commands like 'cd -' that expect OLDPWD to exist.
*
* - Check if OLDPWD exists; if not, create it as an empty variable.
*
* - If OLDPWD exists, clear its value to ensure it remains empty.
*/
void	reset_oldpwd(void)
{
	if (env_var_exists("OLDPWD", shell.dup_envp) == 0)  
		add_empty_env_var("OLDPWD", &shell.dup_envp);  
	else
		clear_env_value("OLDPWD");
}

/**
* @brief Checks if an environment variable exists in the linked list.
* @param var_name name of the variable to check (e.g, "OLDPWD").
* @param env linked list of environment variables.
* @return 1 if the variable exists, 0 otherwise.
*
* - Iterates through the environment list, comparing names.
*
* - Ensures an exact match before returning 1.
*
* - Returns 0 if the variable is not found.
*/
int	env_var_exists(char *var_name, t_list *env)
{
	int		i;
	char	*env_str;

	while (env)
	{
		i = 0;
		env_str = (char *)env->content;
		while (var_name[i] && env_str[i] && (var_name[i] == env_str[i]))
			i++;
		if ((var_name[i] == 0 || ft_isspace(var_name[i]))
			&& (env_str[i] == '=' || env_str[i] == '\0'))
			return (1);
		env = env->next;
	}
	return (0);
}

/**
* @brief Adds a new environment variable to the environment list.
* The variable is created as a new entry in the linked list.
*
* @param var_name The name of the environment variable to create.
* @param env Pointer to the environment variable linked list.
*
* - Duplicates the variable name (e.g., "OLDPWD").
*
* - Creates a new linked list node to store the variable.
*
* - Adds the new variable to the front of the environment list.
*/
void	add_empty_env_var(char *var_name, t_list **env)
{
	t_list	*new_var;
	char	*new_var_str;

	new_var_str = ft_strdup(var_name);
	if (new_var_str == 0)
		quit(EXIT_FAILURE);
	new_var = ft_lstnew(new_var_str);
	if (new_var == 0)
		quit(EXIT_FAILURE);
	ft_lstadd_front(env, new_var);
}

/**
 * @brief Replace a string
 * @param src Source
 * @param dest Destination
 */
void	replace_string(char *src, char **dest)
{
	char	*temp;

	temp = ft_strdup(src);
	if (temp == 0)
		quit(EXIT_FAILURE);
	free(*dest);
	*dest = 0;
	*dest = temp;
}

/**
* @brief Updates SHLVL environment variable everytime we call ./minishell inside
* another ./minishell
* 
* - in case variable is unset we create it and set it to 1
*
* - in case variable is set we increment 1 everytime we call
* ./minishell
*/
void	increment_shlvl(void)
{
	char	*cur_lvl;
	int		lvl_nbr;

	if (env_var_exists("SHLVL", shell.dup_envp) == 0)
		add_empty_env_var("SHLVL=1", &shell.dup_envp);
	else
	{
		cur_lvl = ft_getenv("SHLVL"); //TODO
		lvl_nbr = ft_atoi(cur_lvl);
		lvl_nbr++;
		free(cur_lvl);
		cur_lvl = ft_itoa(lvl_nbr);
		if (cur_lvl == 0)
			quit(EXIT_FAILURE);
		set_env_var("SHLVL", cur_lvl, shell.dup_envp); //TODO
		free(cur_lvl);
	}
}
