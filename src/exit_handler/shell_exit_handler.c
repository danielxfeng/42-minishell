/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_exit_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 18:39:55 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/22 18:49:32 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/mini_env.h"
#include <stdlib.h>
#include <stdio.h>

// @brief print the error, close the env, and exit the program.
void    exit_with_err_shell(t_env **env, char *msg)
{
    perror(msg);
    close_env(env);
    exit(EXIT_FAILURE);       
}

// @brief close the env and return the status code.
//
// @param env: the pointer to env.
// @param status: the status code to return.
//
// @return status code.
int close_and_return(t_env **env, int status)
{
    close_env(env);
    return (status);
}
