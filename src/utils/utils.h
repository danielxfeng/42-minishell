/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 23:56:05 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/17 22:32:13 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "main.h"

void	init_minishell(t_shell *shell, char **envp);

//signals

void	catch_sigint(int signum);
void	catch_sigquit(int signum);

//libft utils

int		ft_putstr(char *str);
int		ft_isspace(char c);
int		ft_strcmp(char *s1, char *s2);
char	*ft_strcpy(char *dest, const char *src);
void	ft_dlstclear(t_dlist **lst, void (*del)(void*));
void	ft_dlstdelone(t_dlist *lst, void (*del)(void*));
t_dlist	*ft_dlstnew(void *data);
void	ft_dlstadd_front(t_dlist **lst, t_dlist *new);
int		ft_strisspace(char *str);
char	*ft_strncat(char *dest, char *src, unsigned int nb);
char	*ft_strcat(char *dest, char *src);
void	free_arr(void **ptr);
char	*ft_strtrim_all(const char *s1, const char *set);

//prompt

void	ft_prompt(void);

//error_message

void	write_shell_error(char *err_msg);

//ft_split_pro
char	**ft_split_pro(char *str, char *charset);
char	*ft_strncpy(char *dest, char *src, size_t n);

//env_utils
char	*ft_getenv(char *key);

#endif