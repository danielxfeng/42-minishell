/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 20:19:51 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/04 20:33:30 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include <stdio.h>

// @brief exit the program because of fatal error.
//
// @param ast: the pointer to ast.
// @param err_code: the return code.
// @param msg: the error msg.
void exit_with_err(t_ast **ast, int err_code, char *msg)
{
    close_ast(ast);
    perror(msg);
    exit(err_code);
}
