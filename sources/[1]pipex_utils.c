/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   [1]pipex_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:06:37 by motero            #+#    #+#             */
/*   Updated: 2022/11/28 23:30:14 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Pipex.h>

void	ft_check_file(char *file, int mode, t_info *info)
{
	if (info->access[0] == 1 || info->access[1] == 1)
		ft_putstr_fd("no such file or directory: ", 2);
	else if (mode == 1)
	{
		if (info->access[0] == 2)
			ft_putstr_fd("permission denied: ", 2);
	}
	else if (mode == 2)
	{
		if (info->access[1] == 2)
			ft_putstr_fd("permission denied: ", 2);
		else
			ft_putstr_fd("is a directory: ", 2);
	}
	ft_putstr_fd(file, 2);
	ft_putstr_fd("\n", 2);
}

void	ft_files(t_info	*info, char **argv, int argc)
{
	if (info->here_doc == 0)
	{	
		info->fd[0] = open(argv[1], O_RDONLY, 0644);
		info->access[0] = 0;
		if (access(argv[1], F_OK))
			info->access[0] = 1;
		else if (access(argv[1], R_OK))
			info->access[0] = 2;
	}
	else
	{
		ft_here_doc(info, argv[2]);
		info->fd[0] = open(".tmp", O_RDONLY, 0644);
		if (info->fd[0] < 0)
			ft_error_3(info);
	}
	if (info->here_doc == 0)
		info->fd[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		info->fd[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	info->access[1] = 0;
	if (access(argv[argc - 1], F_OK))
		info->access[1] = 1;
	else if (access(argv[argc - 1], W_OK))
		info->access[1] = 2;
}

void	ft_child_exec(t_info *info, char *cmd, int pipe_fd[2], int pos)
{
	if ((pos - 2) == 0)
	{
		if (info->fd[0] == -1)
			ft_error_invalid_fd(info, pipe_fd);
		else
		{
			dup2(info->fd[0], STDIN_FILENO);
			dup2(pipe_fd[1], STDOUT_FILENO);
			ft_close_and_exec(pipe_fd, info, cmd);
		}
	}	
	else if ((pos - 1) == info->nbr_cmds)
	{
		if (info->fd[1] == -1)
			ft_error_invalid_fd(info, pipe_fd);
		else
		{
			dup2(info->fd[1], STDOUT_FILENO);
			ft_close_and_exec(pipe_fd, info, cmd);
		}
	}
	else
		dup2(pipe_fd[1], STDOUT_FILENO);
	ft_close_and_exec(pipe_fd, info, cmd);
}

void	ft_close_and_exec(int pipe_fd[2], t_info *info, char *cmd)
{
	close (pipe_fd[1]);
	close (pipe_fd[0]);
	if (info->fd[0] > -1)
		close(info->fd[0]);
	if (info->fd[1] > -1)
		close(info->fd[1]);
	ft_parse_exec_cmd(cmd, info);
}

/** PARSING OF EAACH PATH CONTAINED IN ENV BEFORE CHECKING ACCESS OF COMMANDS**
** 1- look up for line containt PATH and extracting that str **
** 2- Extracting the line without PATH and spliting it by ':' character**
** */
char	**ft_parse_path(char **env)
{
	char	*paths;
	char	**paths_split;
	size_t	i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
	{
		paths_split = malloc(sizeof(char *));
		return (paths_split[0] = 0, paths_split);
	}
	paths = malloc(sizeof(char) * (ft_strlen(env[i]) - 4));
	ft_strlcpy(paths, env[i] + 5, ft_strlen(env[i]) - 5);
	paths_split = ft_split(paths, ':');
	free(paths);
	if (!paths_split)
	{
		ft_putstr_fd("Error: No environement PATH found\n", STDERR_FILENO);
		return (NULL);
	}
	i = -1;
	while (paths_split[++i])
		paths_split[i] = ft_strconcat(paths_split[i], "/");
	return (paths_split);
}
