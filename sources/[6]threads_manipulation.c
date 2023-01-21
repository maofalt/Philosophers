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
	t_thread_info	*info;
	pthread_t		*monitor_thread;

	info = ft_create_info();
	if (!info)
		return ;
	monitor_thread = malloc(sizeof(pthread_t));
	if (!monitor_thread)
		return ;
	current = list;
	gettimeofday(&start, NULL);
	while (current)
	{
		current->info = info;
		if (current->type == PHILOSOPHER)
		{
			ft_init_shared_time(current, start);
			pthread_create(current->thread, NULL, philo_thread, current);
		}
		current = current->next;
		if (current == list || current->args.number_of_philosophers == 1)
			break ;
	}
	pthread_create(monitor_thread, NULL, monitor_philosophers, list);
	pthread_join(*monitor_thread, NULL);
	free(monitor_thread);
}

void	ft_init_shared_time(t_list_item *current, struct timeval start)
{
	current->timestamps->start = start;
	current->timestamps->start_last_meal = start;
	current->timestamps->current = start;
}
