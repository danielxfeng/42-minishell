/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_supp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 09:37:48 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/22 12:39:23 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"

// @brief return the error when there is an option.
//
// @param ast: the pointer to ast tree.
// @param prop: the property of node.
// @return the status code.
int	check_option(t_ast *ast, t_cmd_prop *prop)
{
	if (prop->size < 2)
		return (EXIT_OK);
	if (ft_strlen(ast->tokens[prop->start + 1]) > 1 && ast->tokens[prop->start
			+ 1][0] == '-')
	{
		ft_putstr_fd(ast->tokens[prop->start], 2);
		ft_putstr_fd(": invalid option -- \'", 2);
		ft_putchar_fd(ast->tokens[prop->start + 1][1], 2);
		ft_putstr_fd("\'\n", 2);
		return (EXIT_INVALID_OPTION);
	}
	return (EXIT_OK);
}
