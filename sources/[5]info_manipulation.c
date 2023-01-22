/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   [3]list_manipulation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:06:37 by motero            #+#    #+#             */
/*   Updated: 2023/01/16 23:22:58 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_create_mutexes(t_thread_info *info)
{
	info->display_mutex = malloc(sizeof(pthread_mutex_t));
	if (!info->display_mutex)
	{
		free(info);
		return (1);
	}
	info->death_mutex = malloc(sizeof(pthread_mutex_t));
	if (!info->death_mutex)
	{
		free(info->display_mutex);
		free(info);
		return (1);
	}
	pthread_mutex_init(info->display_mutex, NULL);
	pthread_mutex_init(info->death_mutex, NULL);
	return (0);
}

// A function that creates the linked list of philosophers and forks
t_thread_info	*ft_create_info(void)
{
	t_thread_info	*info;

	info = malloc(sizeof(t_thread_info));
	if (!info)
		return (NULL);
	ft_memset(info, 0, sizeof(t_thread_info));
	info->nbr_philo_full = 0;
	info->someone_died = 0;
	info->end = 0;
	info->launched_threads = 0;
	if (ft_create_mutexes(info))
		return (NULL);
	return (info);
}

void	free_info(t_thread_info *info)
{
	if (info->display_mutex)
	{
		pthread_mutex_destroy(info->display_mutex);
		free(info->display_mutex);
	}
	if (info->death_mutex)
	{
		pthread_mutex_destroy(info->death_mutex);
		free(info->death_mutex);
	}
	free(info);
}
