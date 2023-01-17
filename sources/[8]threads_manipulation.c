/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   [4]threads_manipulation.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:06:37 by motero            #+#    #+#             */
/*   Updated: 2023/01/16 18:36:26 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_create_threads(t_list_item *list)
{
	t_list_item		*current;
	t_thread_info	*thread_info;
	pthread_mutex_t	*display_mutex;
	pthread_mutex_t	*death_mutex;
	struct timeval	start;
	int				*nbr_philo_full;
	int				*someone_died;
	pthread_t		*monitor_thread;

	display_mutex = malloc(sizeof(pthread_mutex_t));
	if (!display_mutex)
		return ;
	death_mutex = malloc(sizeof(pthread_mutex_t));
	if (!death_mutex)
		return ;
	pthread_mutex_init(display_mutex, NULL);
	pthread_mutex_init(death_mutex, NULL);
	current = list;
	gettimeofday(&start, NULL);
	nbr_philo_full = malloc(sizeof(int));
	someone_died = malloc(sizeof(int));
	while (current)
	{
		thread_info = malloc(sizeof(t_thread_info));
		if (!thread_info)
			return ;
		ft_memset(thread_info, 0, sizeof(t_thread_info));
		thread_info->item = current;
		thread_info->display_mutex = display_mutex;
		thread_info->death_mutex = death_mutex;
		thread_info->timestamps = malloc(sizeof(t_timestamps));
		if (!thread_info->timestamps)
			return ;
		ft_memset(thread_info->timestamps, 0, sizeof(t_timestamps));
		thread_info->timestamps->start = start;
		thread_info->timestamps->start_last_meal = start;
		*nbr_philo_full = 0;
		*someone_died = 0;
		thread_info->nbr_philo_full = nbr_philo_full;
		thread_info->someone_died = someone_died;
		if (current->type == PHILOSOPHER)
		{
			pthread_create(current->thread, NULL, philosopher_thread, thread_info);
		}
		current = current->next;
		if (current == list || current->args.number_of_philosophers == 1)
			break ;
	}
	monitor_thread = malloc(sizeof(pthread_t));
	thread_info->item = list;
	pthread_create(monitor_thread, NULL, monitor_philosophers, thread_info);
}
