/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 23:35:04 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/10 22:47:19 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

#include "main.h"

//read_input

char	*read_input(t_dlist *input_history, t_termcaps *termcaps);
int		is_vertical_arrow(char *buf, t_termcaps *termcaps);
void	navigate_history(t_dlist **input_history, t_termcaps *termcaps, char *buf, int *i);
int		has_history(t_dlist *history, t_termcaps *termcaps, char *buf);
void	handle_backspace(t_termcaps *termcaps, char *buf, int *i);

//read_input1
void	reset_cmd_line(char *buf, int *i, t_dlist **history);
void	exit_shell(char *buf, int i);
char	*get_input(char *buf, int i);

//validate_input

int		is_input_valid(const char *input);
int		is_input_valid_unexpected_token(const char *input, char *err_msg);
int		has_open_quote(const char *input, char *err_msg);
int		has_char_at_beginning(const char *input, char c, char *err_msg);
int		has_char_at_end(const char *input, char c, char *err_msg);

//validate_input1

int		has_forbidden_sequence(const char *input, char *test, char *err_msg);
int		has_spaces_between_char(const char *input, char c, char *err_msg);
int		is_input_valid_not_supported(const char *input, char *err_msg);
int		has_non_supported(const char *input, char *test, char *err_msg);
int		has_non_supported_one(const char *input, char *test, char *err_msg);

#endif