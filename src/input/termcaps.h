/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 01:34:28 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/08 14:22:52 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERMCAPS_H
# define TERMCAPS_H

# include "main.h"

int		ft_put(int c);
void	init_termcaps(t_termcaps *termcaps);
int		has_capabilities(t_termcaps *termcaps);
void	disable_canonical_mode(t_termcaps *termcaps);
void	enable_canonical_mode(t_termcaps *termcaps);

#endif