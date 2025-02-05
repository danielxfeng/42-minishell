/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 20:19:51 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/05 21:11:55 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include <stdio.h>
#include <errno.h>

// @brief exit the program because of fatal error.
//
// @param ast: the pointer to ast.
// @param err_code: the return code.
// @param msg: the error msg.
void	exit_with_err(t_ast **ast, int err_code, char *msg)
{
	close_ast(ast);
	perror(msg);
	exit(err_code);
}

// @brief exit the program successfully
//
// @param ast: the pointer to ast.
void exit_without_err(t_ast **ast)
{
	close_ast(ast);
	exit(EXIT_OK);	
}

// @brief return the function with error.
//
// @param err_no: the error number to be set.
// @param rtn_code: the return code.
// @param msg: the message to perror.
// @return rtn_code.
int return_with_err(int err_no, int rtn_code, char *msg)
{
	if (err_no != INVALID_ERR_NO)
		errno = err_no;
	perror(msg);
	return (rtn_code);
}

// @brief close the pipe and return the function with error.
//
// @param msg: the message to perror.
// @param pipe: the fds of a pipe.
// @return rtn_code.
int return_with_err_pipe(char *msg, int *pipe)
{
	if (pipe[0] != -1)
		close(pipe[0]);
	if (pipe[1] != -1)
		close(pipe[1]);	
	return (return_with_err(INVALID_ERR_NO, EXIT_CMD_ERR, msg));
}
