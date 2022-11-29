/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   [4]execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:06:37 by motero            #+#    #+#             */
/*   Updated: 2022/11/28 23:27:07 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Pipex.h>

void	ft_queue_management(t_info *info, char **argv)
{
	int	i;
	int	status;
	int	error_code;

	i = 0;
	while (i < info->nbr_cmds)
	{
		waitpid(info->pid[i++], &status, 0);
		error_code = WEXITSTATUS(status);
		if (error_code == 127)
			ft_putstr_fd("error\n", STDERR_FILENO);
		else if (error_code == 130 || error_code == 2)
			error_code = ft_error_cmd(error_code, i, argv, info);
		else if (error_code == 126)
		{
			if (i == 1)
				ft_check_file(argv[i], 1, info);
			else
				ft_check_file(argv[i + 2 + info->here_doc], 2, info);
		}
		else if (error_code != 0)
			strerror(error_code);
	}
	ft_free_info(info);
	exit (error_code);
}

void	ft_exec_cmd(char *file, char **args, t_info *info)
{
	execve(file, args, info->env);
	if (args)
	{
		while (*args)
			free(*args++);
		free(*args);
	}
}

void	ft_free_double_char(char **str)
{
	int	i;

	i = -1;
	while (str[++i])
		free(str[i]);
	free(str);
}
