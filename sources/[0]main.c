/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   [0]main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:06:37 by motero            #+#    #+#             */
/*   Updated: 2022/11/28 18:37:24 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Pipex.h>

/*CHECK FOR ACCES OF CMD PATH** 
** 1- We test at first if the path is included in the cmd (/,.,~)**
** 2 - if not then, concatente each path in env with the cmd and **
**check for access**
** 3- When acces is correct we send corrrespoding PATH so it can be executed*/
char	*ft_check_access(char **args, char **env)
{
	char	*cmd_path;
	size_t	i;

	if (*args[0] == '/' || *args[0] == '.' || *args[0] == '~')
	{
		cmd_path = args[0];
		if (!(access(cmd_path, (X_OK))))
			return (cmd_path);
	}	
	else
	{
		i = 0;
		while (env[i])
		{
			cmd_path = ft_strjoin(env[i++], args[0]);
			if (!cmd_path)
				ft_error_message("Command construction error\n", EXIT_FAILURE);
			if (!(access(cmd_path, (X_OK))))
				return (cmd_path);
			free(cmd_path);
		}
	}
	ft_free_double_char(args);
	return (NULL);
}

void	ft_parse_exec_cmd(char *cmd, t_info *info)
{
	char	**args;
	char	*file;

	if (cmd[0])
	{
		args = ft_split(cmd, ' ');
		file = ft_check_access(args, info->path);
		if (!file)
		{
			ft_free_info(info);
			exit(130);
		}
		else
			ft_exec_cmd(file, args, info);
	}
	else
		access("", X_OK);
	ft_free_info(info);
	exit(errno);
}

/* 1- Pipe creation and error checking**
** 2- Saving pid into structure, for posteriori queue mgt after forking**
** 3- Test pid value so only child openn anc loses fd properly** 
** 3a-  If first command we swap infile **
** 3b- if last command we swap outfile**
** 3c- Else we swap fd[1] into stoud**
** 4- At the end of the lecture, parent reads anc closes fd properly**
** */
void	ft_redir_pipes(char *cmd, t_info *info, int pos)
{
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		perror("Pipe creation error ");
		exit(EXIT_FAILURE);
	}
	info->pid[pos - 2] = fork();
	if (info->pid[pos - 2] < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (info->pid[pos - 2] == 0)
		ft_child_exec(info, cmd, pipe_fd, pos);
	dup2(pipe_fd[0], STDIN_FILENO);
	close (pipe_fd[0]);
	close (pipe_fd[1]);
}

void	ft_free_info(t_info	*info)
{
	int	i;

	free(info->pid);
	i = -1;
	while (info->path[0] && info->path[++i])
		free(info->path[i]);
	free(info->path);
	if (info->here_doc)
		unlink(".tmp");
	if (info->fd[0] > -1)
		close(info->fd[0]);
	if (info->fd[1] > -1)
		close(info->fd[1]);
	free(info);
}

/* 1- Checking for good nbr of arguments**
** 2- Creation of info structurure allowing (see header) **
** 3- ft_files -> creation and check of ifile & outfile fd's **
** 4- Parse_path --> parsing of env to path (check header)**
** 5- Creation of childs per nbr of commands ** 
** 6- Proper queue mgt of pid as well as error mgt **'
** 7- Properly freeing occupid ressources*/
int	main(int argc, char **argv, char **env)
{
	int		i;
	t_info	*info;
	int		type;

	type = ft_strcmp(argv[1], "here_doc");
	if ((type && argc < 6) || argc < 5)
		return (ft_putstr("Invalid number of arguments."), 1);
	info = malloc(sizeof(t_info));
	if (!info)
		ft_error_message("Malloc error during info creation\n", 1);
	info->here_doc = type;
	info->env = env;
	info->nbr_cmds = argc - 3 - info->here_doc;
	ft_files(info, argv, argc);
	info->path = ft_parse_path(env);
	info->pid = malloc(sizeof(pid_t) * (info->nbr_cmds));
	if (!info->pid)
		ft_error_malloc_pid(info);
	i = 1;
	while (++i < argc - 1 - info->here_doc)
		ft_redir_pipes(argv[i + info->here_doc], info, i);
	ft_queue_management(info, argv);
	return (ft_free_info(info), 1);
}
