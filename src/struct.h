/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 20:03:14 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/08 18:21:56 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

//# include "../lib/libft/inc/libft.h"
# include "libs/libft/libft.h"
# include "main.h"

/**
 * @brief Settings of the terminal and few capabilities from the termcaps
 * library 
 * @param old_term default terminal settings (i.e cronical mode)
 * @param new_term new terminal settings (i.e non-cronical mode)
 * @param buffer the termcaps capabilities require a buffer to be passed to 
 * various functions like tgetent or tgetstr
 * @param key_on set keypad functionalities
 * @param key_off turn off keypad functionalities. Required at end of program
 * @param up_arrow value that the terminal will return when pressing up arrow
 * @param down_arrow value that the terminal will return when pressing down 
 * arrow
 * @param backspace value that the terminal will return when pressing backspace
 * @param del_line capability that allows to delete the content of the line 
 * where the cursor currently is positioned
 * @param set_cursor_begin sets the cursor to the beginning of the current line
 */
typedef struct s_termcaps
{
	struct termios	old_term;
	struct termios	new_term;
	char			*buffer;
	char			*keys_on;
	char			*keys_off;
	char			*up_arrow;
	char			*down_arrow;
	char			*backspace;
	char			*del_line;
	char			*set_cursor_begin;
} 				t_termcaps;

//A double linked list
typedef struct s_dlist
{
	void			*data;
	struct s_dlist	*prev;
	struct s_dlist	*next;
}	t_dlist;

/**
 * @brief A struct carring all data used throughout the program
 * @param history double linkedlist to save each entered input
 * @param terminal struct with the settings of the terminal and a few
 * capabilities
 * @param dup_envp the duplicate of environment variables. Each node is a string
 * @param exixt_status exit_status of the last simple command that ran
 * 
 */
typedef struct s_shell 
{
	//TODO edit for command
	t_dlist			*history;
	t_termcaps		termcaps;
	t_list			*dup_envp;
	int				exit_status;
}	t_shell;

//Single linked list
// typedef struct s_list
// {
// 	void			*data;
// 	struct s_list	*next;
// }	t_list;

#endif