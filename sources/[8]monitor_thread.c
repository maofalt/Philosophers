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
			args = node->args;
			if (args.number_of_times_each_philosopher_must_eat > 0)
			{
				if (*info->nbr_philo_full >= node->args.number_of_philosophers)
				{
					ft_put_down_forks(node);
					stop_philosophers(node);
				}
			}
			if (*info->someone_died >= 1)
			{
				ft_put_down_forks(node);
				stop_philosophers(node);
			}
			if (*info->nbr_philo_full >= args.number_of_philosophers)
			{
				ft_put_down_forks(node);
				stop_philosophers(node);
			}
			if (ft_philo_starved(info->timestamps->start, node))
			{
				ft_put_down_forks(node);
				stop_philosophers(node);
			}
		}
		if (node->args.number_of_philosophers > 1)
			node = node->next;
	}
	pthread_exit(0);
}

void	stop_philosophers(t_list_item *philo)
{
	t_thread_info	*info;
	int				i;

	info = philo->info;
	i = 0;
	while (philo)
	{
		if (philo->type == PHILOSOPHER)
		{
			philo->state = END;
			i++;
		}
		if (i >= philo->args.number_of_philosophers)
			break ;
		philo = philo->next;
	}
	pthread_exit(0);
}
