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
	t_thread_info	*info;

	philo = (t_list_item *)arg;
	info = philo->info;
	while (1)
	{
		pthread_mutex_lock(info->death_mutex);
		if (info->end >= 1)
		{
			pthread_mutex_unlock(info->death_mutex);
			break ;
		}
		pthread_mutex_unlock(info->death_mutex);
		if (philo->state == SLEEPING)
		{
			printf_mutex(philo, 2);
			safe_sleep(philo, philo->args.time_to_sleep);
			philo->state = THINKING;
		}
		if (philo->state == THINKING)
		{
			printf_mutex(philo, 3);
			philo->state = SLEEPING;
		}
		grab_forks(philo);
		gettimeofday(&philo->timestamps->start_last_meal, NULL);
		printf_mutex(philo, 1);
		safe_sleep(philo, philo->args.time_to_eat);
		release_forks(philo);
	}
	release_forks(philo);
	pthread_exit(0);
}

//function that thinks safely, check the  fork variable on the forks and if it's 0, it means that the fork is free and it can take it
void	safe_grab(t_list_item *philo, t_list_item *fork)
{

	while (1)
	{
		pthread_mutex_lock(philo->info->death_mutex);
		if (philo->info->end >= 1)
		{
			pthread_mutex_unlock(philo->info->death_mutex);
			release_forks(philo);
			pthread_exit(0);
		}
		pthread_mutex_unlock(philo->info->death_mutex);
		pthread_mutex_lock(&fork->mutex);
		if (fork->fork == 0)
		{
			fork->fork = 1;
			pthread_mutex_unlock(&fork->mutex);
			break ;
		}
		pthread_mutex_unlock(&fork->mutex);
		check_starved(philo);
	}
}

/*a function tha splits the sleep function into smaller ones so we can 
** check if he died of starvation or if another philospher die*/
void	safe_sleep(t_list_item *philo, long int time_to_sleep)
{
	long int				time_in_ms;
	struct timeval			current;
	struct timeval			start;

	gettimeofday(&start, NULL);
	time_in_ms = 0;
	while (time_in_ms < time_to_sleep)
	{
		pthread_mutex_lock(philo->info->death_mutex);
		if (philo->info->end == 1)
		{
			pthread_mutex_unlock(philo->info->death_mutex);
			release_forks(philo);
			pthread_exit(0);
		}
		pthread_mutex_unlock(philo->info->death_mutex);
		gettimeofday(&current, NULL);
		time_in_ms = (current.tv_sec - start.tv_sec) * 1000
			+ (current.tv_usec - start.tv_usec) / 1000;
		check_starved(philo);
		usleep(150);
	}
	philo->timestamps->current = current;
}

void	printf_mutex(t_list_item *philo, int state)
{
	struct timeval	current;
	t_thread_info	*info;
	int				timestamp;
	int const		i = philo->number;

	info = philo->info;
	pthread_mutex_lock(info->display_mutex);
	pthread_mutex_lock(info->death_mutex);
	if (info->end >= 1)
	{
		pthread_mutex_unlock(info->death_mutex);
		pthread_mutex_unlock(info->display_mutex);
		release_forks(philo);
		pthread_exit(0);
	}
	gettimeofday(&current, NULL);
	timestamp = (current.tv_sec - philo->timestamps->start.tv_sec) * 1000
		+ (current.tv_usec - philo->timestamps->start.tv_usec) / 1000;
	if (state == 0)
		ft_printf("%d %d has taken a fork\n", timestamp, i);
	else if (state == 1)
		ft_printf("%d %d is eating\n", timestamp, i);
	else if (state == 2)
		ft_printf("%d %d is sleeping\n", timestamp, i);
	else if (state == 3)
		ft_printf("%d %d is thinking\n", timestamp, i);
	else if (state == 4)
	{
		ft_printf("%d %d died\n", timestamp, i);
		info->end += 1;
	}
	pthread_mutex_unlock(info->death_mutex);
	pthread_mutex_unlock(info->display_mutex);
}

void	grab_forks(t_list_item *philo)
{
	if (!philo->prev)
	{
		safe_grab(philo, philo->next);
		printf_mutex(philo, 0);
		safe_sleep(philo, philo->args.time_to_die);
		release_forks(philo);
		pthread_exit(0);
	}
	safe_grab(philo, philo->prev);
	printf_mutex(philo, 0);
	safe_grab(philo, philo->next);
	printf_mutex(philo, 0);
}

void	release_forks(t_list_item *philo)
{
	if (philo->prev)
	{
		pthread_mutex_lock(&philo->prev->mutex);
		if (philo->prev->fork == 1)
			philo->prev->fork = 0;
		pthread_mutex_unlock(&philo->prev->mutex);
	}
	pthread_mutex_lock(&philo->next->mutex);
	if (philo->next->fork == 1)
		philo->next->fork = 0;
	pthread_mutex_unlock(&philo->next->mutex);
}

/* fonction that comnpares th current time with the last meal time and 
** if it is greater than the time to die, it kills the philosopher*/
void	check_starved(t_list_item *philo)
{
	struct timeval			current;
	struct timeval const	last_meal = philo->timestamps->start_last_meal;
	long int				time_in_ms;

	gettimeofday(&current, NULL);
	time_in_ms = (current.tv_sec - last_meal.tv_sec) * 1000
		+ (current.tv_usec - last_meal.tv_usec) / 1000;
	if (time_in_ms >= philo->args.time_to_die)
	{
		philo->timestamps->current = current;
		printf_mutex(philo, 4);
		release_forks(philo);
		pthread_exit(0);
	}
}


//createown usleep with  gettimeofday function
inline void	ft_usleep(long int time_in_ms)
{
	struct timeval	start;
	struct timeval	current;
	long int		delta;

	gettimeofday(&start, NULL);
	delta = 0;
	while (delta < time_in_ms)
	{
		gettimeofday(&current, NULL);
		delta = (current.tv_sec - start.tv_sec) * 1000
			+ (current.tv_usec - start.tv_usec) / 1000;
		usleep(time_in_ms / 10);
	}
}
