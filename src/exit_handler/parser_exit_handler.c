/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_exit_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 17:30:58 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/17 18:17:11 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parser.h"

// @brief print out the error, clear the resources, and exit the program.
//
// @param parser: the pointer to parser.
// @param err_code: the return status code.
// @param msg: the error msg to print, will print perror when NULL.
void	exit_with_err_parser(t_parser **parser, int err_code, char *msg)
{
	int	status;

	if (msg)
		perror(msg);
	status = close_the_world(parser);
	if (status == EXIT_OK)
		status = err_code;
	exit(status);
}
