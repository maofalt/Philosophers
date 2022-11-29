/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   [2]error_mgt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:06:37 by motero            #+#    #+#             */
/*   Updated: 2022/11/28 22:21:20 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Pipex.h>

int	ft_error_cmd(int error_code, int i, char **argv, t_info *info)
{
	int	tmp;

	if (ft_strrchr(argv[i + 1], '/') || error_code == 2)
	{
		ft_putstr_fd("no such file or directory: ", STDERR_FILENO);
		tmp = 126;
	}
	else
	{
		ft_putstr_fd("command not found: ", STDERR_FILENO);
		tmp = 127;
	}
	ft_putstr_fd(argv[i + 1 + info->here_doc], STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (tmp);
}	

void	ft_error_malloc_pid(t_info *info)
{
	while (*info->path)
		free(*info->path++);
	free(info->path);
	close(info->fd[0]);
	close(info->fd[1]);
	ft_putstr_fd("Malloc error during pid table creation \n", 2);
}

void	ft_error_message(char *msg, int error)
{
	ft_putstr_fd(msg, 2);
	exit(error);
}

void	ft_error_invalid_fd(t_info *info, int pipe_fd[2])
{
	ft_free_info(info);
	close (pipe_fd[1]);
	close (pipe_fd[0]);
	exit (126);
}
