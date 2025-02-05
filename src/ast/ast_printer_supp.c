/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_printer_supp.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:48:41 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/05 10:36:58 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include <stdlib.h>

// @brief the helper function to print out the information about current node,
// and the given message.
//
// @param curr: the current node.
// @param msg: the given message.
void	print_curr_msg(t_ast_node *node, t_ast_node *curr, char *msg)
{
	if (node == curr)
	{
		printf("*** ");
		if (msg)
			printf(msg);
	}
	printf("\n");
}
