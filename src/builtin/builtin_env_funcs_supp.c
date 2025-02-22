/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env_funcs_supp.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 08:33:10 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/22 12:39:16 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"

// @brief help to print out the env item in `export` output format.
//
// @param item: the env item.
void	print_env_item_for_export(t_env_item *item)
{
	if (item->value)
		printf("declare -x %s=\"%s\"\n", item->key, item->value);
	else
		printf("declare -x %s\n", item->key);
}

// @brief help to print out the env item in `env` output format.
//
// @param item: the env item.
void	print_env_item_for_env(t_env_item *item)
{
	if (item->value)
		printf("%s=%s\n", item->key, item->value);
}

// @brief help to print out the env items.
//
// @param env: the pointer to env.
// @param print_func: the function for printing the env item
//  in different format.
void	print_env(t_env *env, void (*print_func)(t_env_item *))
{
	int	i;

	i = 0;
	while (i < env->size)
		print_func(&(env->items[i++]));
}

// @brief help to check if the char is legal in a env key.
//
// @param c: the char to check.
// @return if the char is legal.
static bool	is_legal_key(char c)
{
	if (c >= 'a' && c <= 'z')
		return (true);
	if (c >= 'A' && c <= 'Z')
		return (true);
	if (c >= '0' && c <= '9')
		return (true);
	if (c == '_')
		return (true);
	return (false);
}

// @brief to check if the param is a valid identifier.
// As a valid identifier:
// - The key consists of 'a-z', 'A-Z' or '0-9';
// - The start char of key can not be number.
//
// @param s: the identifier to check.
// @return the status code.
int	check_identifier(char *s)
{
	int	i;

	if ((s[0] >= '0' && s[0] <= '9') || s[0] == '=')
		return (return_prt_err(EXIT_FAIL, "minishell: export", s,
				"not a valid identifier"));
	i = 0;
	while (s[i])
	{
		if (s[i] == '=')
			break ;
		if (!is_legal_key(s[i]))
			return (return_prt_err(EXIT_FAIL, "minishell: export", s,
					"not a valid identifier"));
		++i;
	}
	return (EXIT_OK);
}
