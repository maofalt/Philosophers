/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   [3]here_doc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:06:37 by motero            #+#    #+#             */
/*   Updated: 2022/11/23 01:12:13 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Pipex.h>

static void	ft_error_1(t_info *info)
{
	perror("ERROR HERE_DOC");
	free(info);
	exit(1);
}

static void	ft_error_2(t_info *info, char *str)
{
	perror("ERROR HERE_DOC");
	free(str);
	free(info);
	exit(1);
}

void	ft_error_3(t_info *info)
{
	unlink(".tmp");
	perror("ERROR HERE_DOC");
	free(info);
	exit(1);
}

void	ft_here_doc(t_info *info, char *lim)
{
	int		tmp;
	char	*line;
	char	*lim1;

	lim1 = ft_strjoin(lim, "\n");
	if (!lim1)
		ft_error_1(info);
	tmp = open(".tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp < 0)
		ft_error_2(info, lim1);
	write(1, "> ", 2);
	line = get_next_line(0);
	while (line && ft_strncmp(line, lim1, ft_strlen(lim1)))
	{
		write(tmp, line, ft_strlen(line));
		free(line);
		write(1, "> ", 2);
		line = get_next_line(0);
	}
	free(line);
	get_next_line(-1);
	free(lim1);
	close(tmp);
}
