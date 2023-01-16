/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   [6]monitor_thread.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:06:37 by motero            #+#    #+#             */
/*   Updated: 2023/01/16 15:22:16 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor_thread(void *arg)
{
	t_thread_info	*info;
	t_list_item		*philosopher;

	info = (t_thread_info *)arg;
	philosopher = info->item;
	while (!ft_stop_signal(info))
	{
		if (*info->someone_died > 0)
		{
			ft_stop_signal(info);
			break ;
		}	
		if (info->args.must_eat_nbr && *info->nbr_philo_full == philosopher->args.nbr_philo)
		{
			ft_stop_signal(info);
			break ;
		}	
		usleep(100);
	}
	return (NULL);
}
