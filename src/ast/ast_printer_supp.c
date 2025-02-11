/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_printer_supp.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:48:41 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/11 11:58:41 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include <stdlib.h>

// @brief the helper function to print out the information about current node,
//
// @param curr: the current node.
void	print_curr_msg(t_ast_node *node, t_ast_node *curr)
{
	if (node == curr)
		printf("*** ");
	printf("\n");
}

// @brief the helper function to print out the indent spaces.
//
// @param level: the indent level.
void	print_level(int level)
{
	int	i;

	i = 0;
	while (i++ < level)
		printf("  ");
}
