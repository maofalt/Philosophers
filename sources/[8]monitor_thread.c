/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   [6]monitor_thread.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:06:37 by motero            #+#    #+#             */
/*   Updated: 2023/01/16 23:18:01 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor_philosophers(void *arg)
{
	t_thread_info	*info;
	t_list_item		*node;
	t_arguments		args;

	node = (t_list_item *)arg;
	info = node->info;
	while (node)
	{
		if (node->type == PHILOSOPHER)
		{
			info = node->info;
			args = node->args;
			if (args.number_of_times_each_philosopher_must_eat >= 0)
			{
				if (args.number_of_times_each_philosopher_must_eat == 0)
				{
					pthread_mutex_lock(info->death_mutex);
					info->nbr_philo_full = node->args.number_of_philosophers;
					pthread_mutex_unlock(info->death_mutex);
				}
				pthread_mutex_lock(info->death_mutex);
				if (info->nbr_philo_full >= node->args.number_of_philosophers)
				{
					pthread_mutex_unlock(info->death_mutex);	
				//	ft_put_down_forks(node);
					stop_philosophers(node);
				}
				pthread_mutex_unlock(info->death_mutex);	
			}
			pthread_mutex_lock(info->death_mutex);
			if (info->end == 1)
			{
				pthread_mutex_unlock(info->death_mutex);
				//ft_put_down_forks(node);
				stop_philosophers(node);
			}
			pthread_mutex_unlock(info->death_mutex);
			// if (ft_philo_starved(node->timestamps->start, node))
			// {
			// 	//ft_put_down_forks(node);
			// 	stop_philosophers(node);
			// }
		}
		if (node->args.number_of_philosophers > 1)
			node = node->next;
	}
	pthread_exit(0);
}

void	stop_philosophers(t_list_item *philo)
{
	pthread_mutex_lock(philo->info->death_mutex);
	philo->info->end = 1;
	pthread_mutex_unlock(philo->info->death_mutex);
	ft_wait_threads(philo);
	pthread_exit(0);
}

