/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stelie <stelie@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 20:10:46 by lbastian          #+#    #+#             */
/*   Updated: 2022/12/12 18:02:40 by lbastian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ms_have_space(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == ' ')
			return (1);
		i++;
	}
	return (0);
}

char	*ms_strjoin_c(char *str, char c)
{
	int		i;
	int		size;
	char	*dest;

	i = 0;
	if (str)
		size = ms_strlen(str);
	else
		size = 0;
	dest = malloc(sizeof(char) * (size + 2));
	if (!dest)
		return (NULL);
	while (str && str[i])
	{
		dest[i] = str[i];
		i++;
	}
	dest[i] = c;
	dest[i + 1] = '\0';
	if (str)
		free(str);
	return (dest);
}

char	*ms_malloc_strjoin_2(char *str1, char *str2)
{
	char	*dest;

	if (!str1)
		dest = malloc(sizeof(char) * (ms_strlen(str2) + 1));
	else
		dest = malloc(sizeof(char) * (ms_strlen(str1) + ms_strlen(str2) + 1));
	if (!dest)
		return (NULL);
	return (dest);
}

char	*ms_strjoin_2(char *str1, char *str2)
{
	char	*dest;
	int		i;
	int		j;

	i = 0;
	j = 0;
	dest = ms_malloc_strjoin_2(str1, str2);
	if (!dest)
		return (NULL);
	while (str1 && str1[i])
	{
		dest[i] = str1[i];
		i++;
	}
	while (str2 && str2[j])
	{
		dest[i + j] = str2[j];
		j++;
	}
	dest[i + j] = '\0';
	if (str1)
		free(str1);
	free(str2);
	return (dest);
}

int	ms_strjoin_3(char *dest, char *str, int start)
{
	int	i;
	int	j;

	j = start;
	i = 0;
	if (str)
	{
		while (str[i])
		{
			dest[j] = str[i];
			i++;
			j++;
		}
	}
	else
		dest = NULL;
	return (j);
}
