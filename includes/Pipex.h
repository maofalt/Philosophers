/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 11:36:43 by motero            #+#    #+#             */
/*   Updated: 2022/11/28 23:27:12 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../gnl/get_next_line.h"

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include "libft.h"
# include <sys/types.h>
# include <sys/wait.h>

/*############################################################################*/
/*                              STRUCTURES                                    */
/*############################################################################*/

typedef struct info
{
	int		fd[2];
	int		access[2];
	char	**env;
	char	**path;
	pid_t	*pid;
	int		nbr_cmds;
	int		here_doc;
}			t_info;

/*############################################################################*/
/*                          MAIN FUNCTIONS                                    */
/*############################################################################*/

char		*ft_check_access(char **args, char **env);
void		ft_parse_exec_cmd(char *cmd, t_info *info);
void		ft_redir_pipes(char *cmd, t_info *info, int pos);
void		ft_free_info(t_info	*info);

/*############################################################################*/
/*                       PIPEX UTILS FUNCTIONS                                */
/*############################################################################*/

void		ft_check_file(char *file, int mode, t_info *info);
void		ft_files(t_info	*info, char **argv, int argc);
void		ft_child_exec(t_info *info, char *cmd, int pipe_fd[2], int pos);
void		ft_close_and_exec(int pipe_fd[2], t_info *info, char *cmd);
char		**ft_parse_path(char **env);

/*############################################################################*/
/*                       ERROR MANAGEMENT FUNCTIONS							*/
/*############################################################################*/

int			ft_error_cmd(int error_code, int i, char **argv, t_info *info);
void		ft_error_malloc_pid(t_info *info);
void		ft_error_message(char *msg, int error);
void		ft_error_invalid_fd(t_info *info, int pipe_fd[2]);

/*############################################################################*/
/*                       	HERE DOC FUNCTIONS							      */
/*############################################################################*/

void		ft_here_doc(t_info *info, char *lim);
void		ft_error_3(t_info *info);

/*############################################################################*/
/*                       	EXECUTION FUNCTIONS							      */
/*############################################################################*/

void		ft_queue_management(t_info *info, char **argv);
void		ft_exec_cmd(char *file, char **args, t_info *info);
void		ft_free_double_char(char **str);

#endif
