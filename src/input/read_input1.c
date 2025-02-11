/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 14:00:13 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/09 15:43:53 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
* Execute the expected bahaviour after Ctrl-C, which is to write "^C" to
* stdout and reset the command line
* @param buf buffer where input chars and ctrl-c have been stored
* @param i size of the buffer before last read. We pass pointer
* because we need to set it to 0
* @param history reference to input_history

* - After pressing Ctrl-C, we need to rewind the input history
*/
void	reset_cmd_line(char *buf, int *i, t_dlist **history)
{
	write(STDOUT_FILENO, "^C\n", ft_strlen("^C\n"));
	shell.exit_status = EXIT_CMD_INTERRUPTED;
	ft_prompt();
	ft_bzero(buf, BUFSIZ);
	*i = 0;
	if (*history)
	{
		while ((*history)->prev)
			*history = (*history)->prev;
	}
}

/**
* @brief Execute the expected behaviour after Ctrl-D, which to write "exit" and to
* exit the shell
* @param buf buffer where input chars and ctrl-d have been stored
* @param i size of the buffer before last read

* - Ctrl-D only exits the program if the cmd line was previously empty

* - We need to overwrite the End of Transmission character from the
* buffer if we don't exit
*/
void	exit_shell(char *buf, int i)
{
	if (i > 0)
	{
		buf[i] = '\0';
		return ;
	}
	ft_putstr_fd("exit\n", STDERR_FILENO);
	quit(shell.exit_status);
}

/**
* @brief Extract the input from the buffer
* @param buf buffer where characters were inputed
* @param i buffer size
* @return input without the line feed

* -	Deleting the line feed from the buffer
*/
char	*get_input(char *buf, int i)
{
	char	*input;

	buf[i - 1] = '\0';
	input = ft_strdup(buf);
	if (!input)
		quit(EXIT_FAILURE);
	return (input);
}
