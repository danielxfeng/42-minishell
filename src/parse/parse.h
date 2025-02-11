/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 13:53:50 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/11 12:41:45 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "main.h"

//parse_utils

int		is_ctrl(char c);
char	*ft_strstr_quotes(char *str, char *to_find);
void	skip_quotes(const char *input, int *curr_pos);
void	skip_spaces(const char *input, int *curr_pos);
int		is_token_delimiter(char c);

//tokenize

char	*get_token(const char *input, int *curr_pos);
char	**tokenize(const char *cmd_line);
int		is_quote(char c);

#endif