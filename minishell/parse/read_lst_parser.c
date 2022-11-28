/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_lst_parser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stelie <stelie@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 15:20:38 by lbastian          #+#    #+#             */
/*   Updated: 2022/11/28 13:32:01 by stelie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ms_read_file_in(t_lst_parser *lst, t_env *st, int read, int write)
{
	int	pip[2];
	int	fd;
	int	fd_pipe;

	fd_pipe = pipe(pip);
	close(read);
	close(write);
	if (fd_pipe == -1)
		return (ft_putmsg_fd(ERR_PIPE, STDERR_FILENO, EXIT_FAILURE));
	lst = lst->next;
	fd = open(lst->value.oper, O_RDONLY);
	if (fd == -1)
	{
		close(pip[0]);
		close(pip[1]);
		return (ft_putmsg_fd(ERR_WRONG_FILE_IN, STDERR_FILENO, EXIT_FAILURE));
	}
	ms_read_lst_parser(lst->next, st, pip[0], pip[1], fd);
	return (EXIT_SUCCESS);
}

static int	ms_read_file_out(t_lst_parser *lst, t_env *st, int read, int write)
{
	int	fd;

	lst = lst->next;
	if (lst->type == ARG_FILE_OUT_OVER)
		fd = open(lst->value.oper, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else
		fd = open(lst->value.oper, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (fd == -1)
	{
		close(read);
		close(write);
		return (ft_putmsg_fd(ERR_WRONG_FILE_OUT, STDERR_FILENO, EXIT_FAILURE));
	}
	ms_read_lst_parser(lst->next, st, read, write, fd);
	return (EXIT_SUCCESS);
}

static int	ms_read_pipe(t_lst_parser *lst, t_env *st, int read, int write)
{
	int	pip[2];
	int	fd_pipe;

	if (lst->next && lst->prev)
	{
		fd_pipe = pipe(pip);
		if (fd_pipe == -1)
			return (ft_putmsg_fd(ERR_PIPE, STDERR_FILENO, EXIT_FAILURE));
		if (!lst->next->next && lst->next->type == CMD)
		{
			ms_is_builtin_dumb(lst->next->value.cmd, st, read, write, pip[0], pip[1], CMD_END);
			return (0);
		}
		else if (lst->next->type == CMD)
			ms_is_builtin_dumb(lst->next->value.cmd, st, read, write, pip[0], pip[1], CMD_MIDDLE);
		lst = lst->next;
		ms_read_lst_parser(lst->next, st, pip[0], pip[1], 1);
	}
	else
	{
		close (read);
		close (write);
		return (ft_putmsg_fd(ERR_PIPE_CMD, STDERR_FILENO, EXIT_FAILURE));
	}
	return (EXIT_SUCCESS);
}

int	ms_read_lst_parser(t_lst_parser *lst, t_env *st, int read, int write, int fd2)
{
	int	pip[2];

	if (lst && lst->prev == NULL && lst->next == NULL && lst->type == CMD)
	{
		pipe(pip);
		ms_is_builtin_dumb(lst->value.cmd, st, read, write, pip[0], pip[1], CMD_END);
		return (EXIT_SUCCESS);
	}
	if (lst && lst->type == CMD && fd2 == 0)
	{
		ms_is_builtin_dumb(lst->value.cmd, st, 0, 1, read, write, CMD_BEGIN);
		lst = lst->next;
	}
	if (lst && lst->prev && lst->prev->type == ARG_FILE_IN && lst->type == CMD)
	{
		if (!lst->next)
			ms_is_builtin_dumb(lst->value.cmd, st, fd2, 1, read, write, CMD_FILE_IN_END);
		else
			ms_is_builtin_dumb(lst->value.cmd, st, fd2, 1, read, write, CMD_FILE_IN);
		lst = lst->next;
	}
	if (lst && lst->prev && lst->type == CMD && (lst->prev->type == ARG_FILE_OUT_OVER || lst->prev->type == ARG_FILE_OUT_APP))
	{
		if (!lst->next)
			ms_is_builtin_dumb(lst->value.cmd, st, read, 1, write, fd2, CMD_FILE_OUT_END);
		else
			ms_is_builtin_dumb(lst->value.cmd, st, read, 1, write, fd2, CMD_FILE_OUT);
		lst = lst->next;
	}
	if (lst && lst->next && lst->type == CMD)
		lst = lst->next;
	if (lst && lst->type == PIPE)
		if (ms_read_pipe(lst, st, read, write) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	if (lst && lst->type == FILE_IN)
		if (ms_read_file_in(lst, st, read, write) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	if (lst && (lst->type == FILE_OUT_OVER || lst->type == FILE_OUT_APP))
		if (ms_read_file_out(lst, st, read, write) == EXIT_FAILURE)
			return (EXIT_FAILURE);

	return (0);
}
