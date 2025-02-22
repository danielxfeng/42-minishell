/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 21:54:27 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/10 23:08:29 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static size_t	get_length(char **strs)
{
	size_t	len;

	len = 0;
	while (*strs)
	{
		len += ft_strlen(*strs);
		strs++;
	}
	return (len);
}

static void	concat_all(char *dest, char **strs)
{
	int	i;

	i = 0;
	while (strs[i])
	{
		ft_strcat(dest, strs[i]);
		i++;
	}
}

void	free_arr(void **ptr)
{
	int	i;

	if (!ptr)
		return ;
	i = 0;
	while (ptr[i])
		free(ptr[i++]);
	free(ptr);
}

char	*ft_strcat(char *dest, char *src)
{
	char	*sav_d;

	sav_d = dest;
	while (*dest)
		dest++;
	while (*src)
		*dest++ = *src++;
	*dest = '\0';
	return (sav_d);
}


char	*ft_strtrim_all(const char *s1, const char *set)
{
	char	*new_str;
	char	**split;
	size_t	size_new_str;

	if (!s1)
		return (0);
	split = ft_split_pro((char *)s1, (char *)set);
	if (!split)
		return (0);
	size_new_str = get_length(split);
	if (size_new_str == 0)
		return (ft_strdup(""));
	new_str = ft_calloc(size_new_str + 1, sizeof(char));
	if (!new_str)
		return (0);
	concat_all(new_str, split);
	free_arr((void **)split);
	return (new_str);
}
