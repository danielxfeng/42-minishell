/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 07:45:10 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/06 08:53:27 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"

// @brief if the given cmd a built-in function
// TODO to implement.
//
// @param the given cmd to be matched.
// @return if the function is built-in.
bool is_builtin_func(char *cmd)
{
    return (false);
}

// @brief execute the built-in function.
// TODO to implement.
//
// @param tokens: the tokens.
// @param start: the index of command.
// @param size: the size of argv.
// @return the status code.
int exec_builtin_func(char **tokens, int start, int size)
{
    return (EXIT_OK);
}
