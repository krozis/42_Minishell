/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stelie <stelie@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 20:08:43 by lbastian          #+#    #+#             */
/*   Updated: 2022/11/21 16:58:28 by lbastian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_size_env_var(t_env *st, char *str, int index, int fact)
{
	int		lenght;
	int		start;
	char	*temp;
	int		size;

	size = 0;
	start = ms_start_read_var(str, index);
	lenght = ms_lenght_read_var(str, index);
	if (lenght == 0 && fact == 1)
		return (index + 1);
	if (fact == 1)
		return (start + lenght);
	temp = ft_substr(str, start + 1, lenght);
	while (st)
	{
		if (ft_strcmp(st->var, temp) == 0)
		{
			size = ms_strlen(st->value);
			break ;
		}
		st = st->next;
	}
	free(temp);
	return (size);
}

int	ft_split_expand(t_list **lst, char **split)
{
	int	i;

	i = 1;
	if (!split)
		return (1);
	(*lst)->content = ms_strjoin_2(((*lst)->content), split[0]);
	free(split[0]);
	while (split[i])
	{
		if (ms_lstadd(lst, ms_lst_new_join(split[i], CMD)))
			return (1);
		(*lst) = (*lst)->next;
		free(split[i]);
		i++;
	}
	free(split);
	return (0);
}

int	ft_error_return(char *str)
{
	free(str);
	return (1);
}

int	ms_set_quotes(char *str, int i, int quotes)
{
	if (str[i] == '\'' && quotes == 0)
		quotes = 1;
	else if (str[i] == '"' && quotes == 0)
		quotes = 2;
	else if ((str[i] == '\'' && quotes == 1)
		|| (str[i] == '"' && quotes == 2))
		quotes = 0;
	return (quotes);
}

int	ms_replace_loop(t_list **lst, t_env *st, char *str, int *i, int quotes)
{
	int		j;
	char	*temp;

	(*i)++;
	if (str[(*i)] && str[(*i)] != '\'' && str[(*i)] != '"' && str[(*i)] != '$'
		&& str[(*i)] != ' ')
	{
		j = (*i);
		while (str[(*i)] && str[(*i)] != '\'' && str[(*i)] != '"'
			&& str[(*i)] != '$' && str[(*i)] != ' ')
			(*i)++;
		temp = ms_find_var((ft_substr(str, j, (*i) - j)), st);
		if (!temp)
			return (ft_error_return(str));
		if (quotes == 2 || ms_strlen(temp) == 0)
			(*lst)->content = ms_strjoin_2((*lst)->content, temp);
		else
			if (ft_split_expand(lst, ft_split(temp, ' ')))
				return (ft_error_return(str));
	}
	else
		(*lst)->content = ms_strjoin_c((*lst)->content, '$');
	return (0);
}

int	ft_replace(t_list **lst, t_env *st, int i, int quotes)
{
	char	*str;

	str = ms_strcpy_2((*lst)->content);
	if (!str)
		return (1);
	free((*lst)->content);
	(*lst)->content = NULL;
	while (str[i])
	{
		quotes = ms_set_quotes(str, i, quotes);
		if (str[i] == '$' && quotes != 1)
		{
			if (ms_replace_loop(lst, st, str, &i, quotes))
				return (1);
		}
		else
		{
			(*lst)->content = ms_strjoin_c((*lst)->content, str[i]);
			i++;
		}
		if (!(*lst)->content)
			return (ft_error_return(str));
	}
	free(str);
	return (0);
}
/*
   int	ft_replace(t_list **lst, t_env *st)
   {
   char	*temp;
   char	*str;
   int		i;
   int		j;
   int		quotes;

   i = 0;
   quotes = 0;
   str = ms_strcpy_2((*lst)->content);
   if (!str)
   return (1);
   free((*lst)->content);
   (*lst)->content = NULL;
   temp = NULL;
   while (str[i])
   {
   quotes = ms_set_quotes(str, i, quotes);
   if (str[i] == '$' && quotes != 1)
   {
   i++;
   if (str[i] && str[i] != '\'' && str[i] != '"' && str[i] != '$'
   && str[i] != ' ')
   {
   j = i;
   while (str[i] && str[i] != '\'' && str[i] != '"'
   && str[i] != '$' && str[i] != ' ')
   i++;
   temp = ms_find_var((ft_substr(str, j, i - j)), st);
   if (!temp)
   return (ft_error_return(str));
   if (quotes == 2 || ms_strlen(temp) == 0)
   (*lst)->content = ms_strjoin_2((*lst)->content, temp);
   else
   if (ft_split_expand(lst, ft_split(temp, ' ')))
   return (ft_error_return(str));
   }
   else
   (*lst)->content = ms_strjoin_c((*lst)->content, '$');
   }
   else
   {
   (*lst)->content = ms_strjoin_c((*lst)->content, str[i]);
   i++;
   }
   if (!(*lst)->content)
   return (ft_error_return(str));
   }
   free(str);
   return (0);
   }*/

int	ms_main_replace_env(t_list **lst, t_env *st)
{
	t_list	*temp;

	temp = (*lst);
	while (lst && (*lst))
	{
		if ((*lst)->type != LIMITOR)
			if (ft_replace(lst, st, 0, 0) || !(*lst)->content)
				return (1);
		(*lst) = (*lst)->next;
	}
	(*lst) = temp;
	return (0);
}
