/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_exit_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 17:30:58 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/22 12:30:00 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parser.h"
#include "../libs/libft/libft.h"
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
	if (parser && *parser && (*parser)->env)
	{
		close_env(&(*parser)->env);
		(*parser)->env = NULL;
	}
	close_parser(parser, true);
	exit(err_code);
}

// @brief print out the error, clear the parser, and return.
//
// @param parser: the pointer to parser.
// @param err_code: the return status code.
// @param msg: the error msg to print.
// @return: the error code.
int	return_with_err_parser(t_parser **parser, int err_code, char *msg)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\'\n", 2);
	close_parser(parser, true);
	return (err_code);
}
