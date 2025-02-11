/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 23:34:32 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/11 13:29:32 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
* Reads user input in real-time, handling various key interactions in a shell-like interface.
* This function processes single keypresses due to non-canonical terminal mode, enabling
* immediate handling of special keys such as arrow keys, backspace, and control sequences.
*
* Special Cases:

* - Up/Down Arrow: Navigates through command history without modifying input.

* - Backspace: Removes the last character from input dynamically.

* - Ctrl-C : Clears the current input and resets the command line.

* - Ctrl-D : Exits the shell gracefully.

* - Special Keys (Home, Del, etc.): Ignored to prevent unintended input corruption.
*
* The function continuously reads characters into a buffer until a newline (`\n`) is encountered,
* at which point it extracts and returns the full input string.
*
* @param input_history - Doubly linked list storing previous commands for history navigation.
* @param termcaps - Struct containing terminal escape sequences for special key handling.
* @return Pointer to the final input string (allocated dynamically).
*/
char	*read_input(t_dlist *history, t_termcaps *termcaps)
{
	char	*input;
	char	buf[BUFSIZ];
	int		i;
	int		nb_char_read;

	i = 0;
	ft_bzero(buf, BUFSIZ);
	while (!ft_strchr(buf, '\n'))
	{
		nb_char_read = read(STDIN_FILENO, &buf[i], BUFSIZ - i);
		if (is_vertical_arrow(&buf[i], termcaps))
			navigate_history(&history, termcaps, buf, &i);
		else if (!ft_strcmp(&buf[i], termcaps->backspace))
			handle_backspace(termcaps, buf, &i);
		else if (nb_char_read > 2 || (nb_char_read == 1 && is_ctrl(buf[i])))
			ft_bzero(&buf[i], BUFSIZ - i);
		else if (buf[i] == CTRL_C)
			reset_cmd_line(buf, &i, &history);
		else if (buf[i] == CTRL_D)
			exit_shell(buf, i);
		else
			i += write(STDOUT_FILENO, &buf[i], nb_char_read);
	}
	input = get_input(buf, i);
	return (input);
}

/**
* @brief navigate input history, set the buffer to the value of the history
* and update the index based on the length of that value
* @param history double linked list with an input previously entered
* @param termcaps struct with terminal capabilities
* @param buf buffer where the history input will be set
* @param i index where we are in the buffer
* -	check if history exixt

* - Case: up arrow + only node in the list

* - Case: up arrow + more than one node in list

* - Case: down arrow + more than one node in list

* - Delete the current line from STDOUT

* - Set cursor at the beginning of current line

* - While we are writting the new input, we change the value of *i
*	so that the next char input while be set after the input just wrote
*/
void	navigate_history(t_dlist **history, t_termcaps *termcaps, char *buf, int *i)
{
	char	*input;

	if (!has_history(*history, termcaps, &buf[*i]))
	{
		ft_bzero(&buf[*i], BUFSIZ - *i);
		return ;
	}
	if (!ft_strcmp(termcaps->up_arrow, &buf[*i]) && !(*history)->next)
		input = (*history)->data;
	else if (!ft_strcmp(termcaps->up_arrow, &buf[*i]))
	{
		input = (*history)->data;
		*history = (*history)->next;
	}
	else if (!ft_strcmp(termcaps->down_arrow, &buf[*i]))
	{
		*history = (*history)->prev;
		input = (*history)->data;
	}
	tputs(termcaps->del_line, 1, ft_put);
	tputs(termcaps->set_cursor_begin, 1, ft_put);
	ft_prompt();
	ft_bzero(buf, BUFSIZ);
	ft_strcpy(buf, input);
	*i = write(STDOUT_FILENO, buf, ft_strlen(buf));
}

/**
* Checks if, according to the keypressed, there is a history to show
* @param history double linked list with an input previously entered
* as node
* @param termcaps struct with terminal capabilities capabilities
* @param buf buffer where the user input is set

* - If we press down arrow, we need to make sure that we are not at the
* beginning of the list
*/
int	has_history(t_dlist *history, t_termcaps *termcaps, char *buf)
{
	int	check;

	if (!history)
		check = 0;
	else if (!ft_strcmp(termcaps->down_arrow, buf) && !history->prev)
		check = 0;
	else
		check = 1;
	return (check);
}

/**
* @brief Checks if the characters in buffer are up or down arrow
* @param buf Buffer reprenting one or more characters
* @param termcaps Struct with terminal capabilities
*/
int	is_vertical_arrow(char *buf, t_termcaps *termcaps)
{
	int	check;

	if (!ft_strcmp(termcaps->up_arrow, buf))
		check = 1;
	else if (!ft_strcmp(termcaps->down_arrow, buf))
		check = 1;
	else
		check = 0;
	return (check);
}

/**
* Deletes the last characters inputed
* @param termcaps struct with terminal capabilities
* @param buf buffer where the history input will be set
* @param i index where we are in the buffer

* -	We are deleting both the last char input and the backspace ANSI code

* - Extended ASCII characters take 2 bytes

* - Delete the current line from STDOUT

* - Set cursor at the beginning of current line
*/
void	handle_backspace(t_termcaps *termcaps, char *buf, int *i)
{
	int	nb_char_to_delete;

	if (*i == 0 || ft_isascii(buf[*i - 1]))
		nb_char_to_delete = 1;
	else
		nb_char_to_delete = 2;
	ft_bzero(&buf[*i - nb_char_to_delete], BUFSIZ - *i + nb_char_to_delete);
	tputs(termcaps->del_line, 1, ft_put);
	tputs(termcaps->set_cursor_begin, 1, ft_put);
	ft_prompt();
	*i = write(STDOUT_FILENO, buf, ft_strlen(buf));
}
