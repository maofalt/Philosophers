/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   [5]philosophers_behavior.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:06:37 by motero            #+#    #+#             */
/*   Updated: 2023/01/16 22:26:30 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// A function that runs in a separate thread for each philosopher
void	*philo_thread(void *arg)
{
	t_list_item		*philo;

	philo = (t_list_item *)arg;
	while (philo->info->end == 0)
	{
		if (philo->state == THINKING)
		{
			if (printf_mutex(philo, 3))
				return (NULL);
			philo->state = SLEEPING;
		}
		if (philo->args.number_of_philosophers == 5)
			ft_usleep(philo->args.time_to_sleep);
		if (philo_eats(philo))
			return (NULL);
		if (philo->state == SLEEPING)
		{
			if (printf_mutex(philo, 2))
				return (NULL);
			if (safe_sleep(philo, philo->args.time_to_sleep))
				return (NULL);
			philo->state = THINKING;
		}
	}
	return (NULL);
}

int	philo_eats(t_list_item *philo)
{
	if (grab_forks(philo))
		return (1);
	gettimeofday(&philo->timestamps->start_last_meal, NULL);
	if (printf_mutex(philo, 1))
		return (1);
	if (safe_sleep(philo, philo->args.time_to_eat))
		return (1);
	philo->times_eaten++;
	if (philo->times_eaten
		== philo->args.number_of_times_each_philosopher_must_eat)
		philo->info->nbr_philo_full++;
	if (philo->info->nbr_philo_full == philo->args.number_of_philosophers)
		philo->info->end += 1;
	release_forks(philo);
	return (0);
}

int	grab_fork(t_list_item *fork, int *forks, int fork_mask)
{
	int	forks_held;

	forks_held = 0;
	pthread_mutex_lock(&fork->mutex);
	if (fork->fork == 0)
	{
		fork->fork = 1;
		*forks = *forks + 1;
		forks_held = fork_mask;
	}
	pthread_mutex_unlock(&fork->mutex);
	return (forks_held);
}

/*a function tha splits the sleep function into smaller ones so we can 
** check if he died of starvation or if another philospher die*/
int	safe_sleep(t_list_item *philo, long int time_to_sleep)
{
	long int				time_in_ms;
	struct timeval			current;
	struct timeval			start;

	gettimeofday(&start, NULL);
	time_in_ms = 0;
	while (time_in_ms < time_to_sleep)
	{
		if (philo->info->end == 1)
			return (1);
		gettimeofday(&current, NULL);
		time_in_ms = (current.tv_sec - start.tv_sec) * 1000
			+ (current.tv_usec - start.tv_usec) / 1000;
		if (check_starved(philo))
			return (1);
		usleep(1000);
	}
	philo->timestamps->current = current;
	return (0);
}

int	printf_mutex(t_list_item *philo, int state)
{
	struct timeval			curr;
	int						ts;
	struct timeval const	st = philo->timestamps->start;

	pthread_mutex_lock(philo->info->display_mutex);
	if (philo->info->end >= 1)
		return (pthread_mutex_unlock(philo->info->display_mutex), 1);
	gettimeofday(&curr, NULL);
	ts = (curr.tv_sec - st.tv_sec) * 1000 + (curr.tv_usec - st.tv_usec) / 1000;
	if (state == 0)
		ft_printf("%d %d has taken a fork\n%d %d has taken a fork\n",
			ts, philo->number, ts, philo->number);
	else if (state == 1)
		ft_printf("%d %d is eating\n", ts, philo->number);
	else if (state == 2)
		ft_printf("%d %d is sleeping\n", ts, philo->number);
	else if (state == 3)
		ft_printf("%d %d is thinking\n", ts, philo->number);
	else if (state == 4)
		death_message(philo, ts);
	else if (state == 5)
		ft_printf("%d %d has taken a fork\n", ts, philo->number);
	return (pthread_mutex_unlock(philo->info->display_mutex), 0);
}
