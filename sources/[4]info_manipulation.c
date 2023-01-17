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

// A function that creates the linked list of philosophers and forks
void	*ft_create_info(t_list_item *list)
{
	t_thread_info	*info;

	info = malloc(sizeof(t_thread_info));
	if (!info)
	{
		info = NULL;
		return ;
	}
	ft_memset(info, 0, sizeof(t_thread_info));
	info->timestamps = ft_calloc(1, sizeof(t_timestamps));
	if (!info->timestamps)
	{
		free(info);
		info = NULL;
		return ;
	}
	info->nbr_philo_full = 0;
	info->someone_died = 0;
	info->display_mutex = malloc(sizeof(pthread_mutex_t));
	if (!info->display_mutex)
	{
		free(info->timestamps);
		free(info);
		info = NULL;
		return ;
	}
	info->death_mutex = malloc(sizeof(pthread_mutex_t));
	if (!info->death_mutex)
	{
		free(info->display_mutex);
		free(info->timestamps);
		free(info);
		info = NULL;
		return ;
	}
	pthread_mutex_init(info->display_mutex, NULL);
	pthread_mutex_init(info->death_mutex, NULL);
}