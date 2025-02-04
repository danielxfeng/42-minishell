/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_close_supp.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:11:40 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/04 19:23:04 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include <stdlib.h>

// Destructor of path.
void close_path(t_ast *ast)
{
    int i;

    if (!(ast->path))
        return;   
    i = 0;
    while ((ast->path)[i])
	{
		free((ast->path)[i]);
		(ast->path)[i++] = NULL;
	}
	free(ast->path);
	ast->path = NULL;
}

// Destructor of tokens.
void close_tokens(t_ast *ast)
{
    int i;

    if (!(ast->tokens) || ast->tk_size <= 0)
        return;  
    i = 0;
    while (i < ast->tk_size)
	{
		free((*ast->tokens)[i]);
		(ast->tokens)[i++] = NULL;
	}
    free(ast->tokens);
	ast->tokens = NULL;
}
