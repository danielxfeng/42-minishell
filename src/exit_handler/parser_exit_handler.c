/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_exit_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 17:30:58 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/17 21:18:16 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parser.h"
#include <stdio.h>
#include <stdlib.h>

// @brief print out the error, clear the resources, and exit the program.
//
// @param parser: the pointer to parser.
// @param err_code: the return status code.
// @param msg: the error msg to print, will print perror when NULL.
void	exit_with_err_parser(t_parser **parser, int err_code, char *msg)
{
	if (msg)
		perror(msg);
	close_parser(parser, true);
	exit(err_code);
}
