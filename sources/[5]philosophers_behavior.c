/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   [5]philosophers_behavior.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:06:37 by motero            #+#    #+#             */
/*   Updated: 2022/12/07 23:10:47 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// A function that runs in a separate thread for each philosopher
void	*philosopher_thread(void *arg)
{
	t_list_item	*philosopher;
	int			number_of_times_eaten;

	philosopher = (t_list_item *)arg;
	number_of_times_eaten = 0;
	while (philosopher->state != DEAD && (number_of_times_eaten < args->number_of_times_each_philosopher_must_eat ||
			args->number_of_times_each_philosopher_must_eat == 0))
	{
		philosopher->state = THINKING;
		printf("%d %d is thinking\n", timestamp_in_ms(), philosopher->number);
		usleep(args->time_to_sleep * 1000);
		pthread_mutex_lock(&philosopher->prev->mutex);
		pthread_mutex_lock(&philosopher->next->mutex);
		printf("%d %d has taken a fork\n", timestamp_in_ms(), philosopher->number);
		philosopher->state = EATING;
		printf("%d %d is eating\n", timestamp_in_ms(), philosopher->number);
		usleep(args->time_to_eat * 1000);
		pthread_mutex_unlock(&philosopher->prev->mutex);
		pthread_mutex_unlock(&philosopher->next->mutex);
		number_of_times_eaten++;
	}
	philosopher->state = DEAD;
	printf("%d %d died\n", timestamp_in_ms(), philosopher->number);
	return (NULL);
}
