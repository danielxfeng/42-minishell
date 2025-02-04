/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_print_supp.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:48:41 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/04 19:49:42 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include <stdlib.h>

void print_curr_msg(t_ast_node *node, t_ast_node *curr, char *msg)
{
    if (node == curr)
	{
		printf("*** ");
		if (msg)
			printf(msg);
	}
	printf("\n");
}
