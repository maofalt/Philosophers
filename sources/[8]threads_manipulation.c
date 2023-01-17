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
	struct timeval	start;
	int				*nbr_philo_full;
	int				*someone_died;
	pthread_t		*monitor_thread;

	current = list;
	gettimeofday(&start, NULL);
	while (current)
	{
		current->info->timestamps->start = start;
		current->info->timestamps->start_last_meal = start;
		*nbr_philo_full = 0;
		*someone_died = 0;
		thread_info->nbr_philo_full = nbr_philo_full;
		thread_info->someone_died = someone_died;
		if (current->type == PHILOSOPHER)
			pthread_create(current->thread, NULL, philosopher_thread, thread_info);
		current = current->next;
		if (current == list || current->args.number_of_philosophers == 1)
			break ;
	}
	monitor_thread = malloc(sizeof(pthread_t));
	thread_info->item = list;
	pthread_create(monitor_thread, NULL, monitor_philosophers, thread_info);
}
