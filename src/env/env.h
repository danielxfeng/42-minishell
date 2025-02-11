/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 01:08:30 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/07 16:40:43 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "main.h"

//env

void	copy_env(t_list **dup_envp, char **envp);
void	clear_env_value(char *var_name);
char	*ft_getenv(char *var_name);

//env_utils

void	reset_oldpwd(void);
int		env_var_exists(char *var_name, t_list *env);
void	add_empty_env_var(char *var_name, t_list **env);
void	replace_string(char *src, char **dest);
void	increment_shlvl(void);

//env_utils2

void	set_env_var(char *var, char *new_value, t_list *env);
char	*join_env_value(char **envp, char *var_name, char *new_value);

#endif