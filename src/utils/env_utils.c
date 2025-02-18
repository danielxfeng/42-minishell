/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:31:13 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/18 14:02:09 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main.h"

char	*ft_getenv(char *key)
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
		while (key[i] && curr_envp[i] && (key[i] == curr_envp[i]))
			i++;
		if (!key[i] && (curr_envp[i] == '='
				|| curr_envp[i] == '\0'))
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
