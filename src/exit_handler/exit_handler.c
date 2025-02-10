/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 20:19:51 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/10 19:14:10 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void	exit_without_err(t_ast **ast)
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
int	return_with_err(int err_no, int rtn_code, char *msg)
{
	if (err_no != INVALID_ERR_NO)
		errno = err_no;
	perror(msg);
	return (rtn_code);
}

// @brief return the function with printing error.
//
// @param rtn_code: the return code.
// @param msg: the message to print.
// @return rtn_code.
int return_prt_err(int rtn_code, char *cmd, char *filename, char *msg)
{
	if (cmd)
    {
        ft_putstr_fd(cmd, 2);
        ft_putstr_fd(": ", 2);
    }
    if (filename)
    {
        ft_putstr_fd(filename, 2);
        ft_putstr_fd(": ", 2);        
    }
    if (msg)
        ft_putstr_fd(msg, 2);
    else
        ft_putstr_fd(strerror(errno), 2);  
    ft_putstr_fd("\n", 2);
	return (rtn_code);
}
