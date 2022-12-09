/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   [6]monitor_thread.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:06:37 by motero            #+#    #+#             */
/*   Updated: 2022/12/09 22:12:47 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *monitor_thread(void *arg)
{
	t_thread_info	*info = (t_thread_info *)arg;
	t_list_item		*philosopher = info->item;
	
	while (!ft_stop_signal(info))
	{
	  // Check if any philosophers have died
	  if (*info->someone_died > 0)
	  {
	    // Stop all philosophers
	    ft_stop_signal(info);
	    break;
	  }	
	  // Check if all philosophers have eaten enough
	  if (info->args.must_eat_nbr && *info->nbr_philo_full == philosopher->args.nbr_philo)
	  {
	    // Stop all philosophers
	    ft_stop_signal(info);
	    break;
	  }	
	  // Sleep for a short period of time before checking again
	  usleep(100);
	}

  return NULL;
}
