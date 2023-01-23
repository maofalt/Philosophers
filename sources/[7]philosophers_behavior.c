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
	while (info->end == 0)
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

/* function that thinks safely, check the  fork variable on the forks 
** and if it's 0, it means that the fork is free and it can take it */
int	safe_grab(t_list_item *philo, t_list_item *fork)
{
	while (1)
	{
		if (philo->info->end >= 1)
			return (1);
		pthread_mutex_lock(&fork->mutex);
		if (fork->fork == 0)
		{
			fork->fork = 1;
			pthread_mutex_unlock(&fork->mutex);
			break ;
		}
		pthread_mutex_unlock(&fork->mutex);
		if (check_starved(philo))
			return (1);
	}
	return (0);
}

int	safe_grab_two(t_list_item *philo)
{
	int					both_forks;
	int					forks_held;
	t_list_item			*left_fork;
	t_list_item			*right_fork;

	left_fork = philo->prev;
	right_fork = philo->next;
	both_forks = 0;
	forks_held = 0;
	while (both_forks < 2)
	{
		forks_reset(forks_held, left_fork, right_fork);
		if (check_starved(philo) || philo->info->end >= 1)
			return (1);
		both_forks = 0;
		forks_held = 0;
		forks_held |= grab_fork(left_fork, &both_forks, 1);
		if (forks_held & 1)
			forks_held |= grab_fork(right_fork, &both_forks, 2);
		usleep(100);
	}
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

void	forks_reset(int forks_held, t_list_item *l_fork, t_list_item *r_fork)
{
	if (forks_held & 1)
	{
		pthread_mutex_lock(&l_fork->mutex);
		l_fork->fork = 0;
		pthread_mutex_unlock(&l_fork->mutex);
	}
	if (forks_held & 2)
	{
		pthread_mutex_lock(&r_fork->mutex);
		r_fork->fork = 0;
		pthread_mutex_unlock(&r_fork->mutex);
	}
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
	struct timeval	current;
	t_thread_info	*info;
	int				timestamp;
	int const		i = philo->number;

	info = philo->info;
	pthread_mutex_lock(info->display_mutex);
	if (info->end >= 1)
	{
		pthread_mutex_unlock(info->display_mutex);
		return (1);
	}
	gettimeofday(&current, NULL);
	timestamp = (current.tv_sec - philo->timestamps->start.tv_sec) * 1000
		+ (current.tv_usec - philo->timestamps->start.tv_usec) / 1000;
	if (state == 0)
	{
		ft_printf("%d %d has taken a fork\n", timestamp, i);
		ft_printf("%d %d has taken a fork\n", timestamp, i);
	}
	else if (state == 1)
	{
		ft_printf("%d %d is eating\n", timestamp, i);
	}
	else if (state == 2)
		ft_printf("%d %d is sleeping\n", timestamp, i);
	else if (state == 3)
		ft_printf("%d %d is thinking\n", timestamp, i);
	else if (state == 4)
	{
		ft_printf("%d %d died\n", timestamp, i);
		info->end += 1;
	}
	else if (state == 5)
		ft_printf("%d %d has taken a fork\n", timestamp, i);
	pthread_mutex_unlock(info->display_mutex);
	return (0);
}

int	grab_forks(t_list_item *philo)
{
	if (!philo->prev)
	{
		if (safe_grab(philo, philo->next))
			return (1);
		if (printf_mutex(philo, 5))
			return (1);
		if (safe_sleep(philo, philo->args.time_to_die))
			return (1);
		return (1);
	}
	safe_grab_two(philo);
	if (printf_mutex(philo, 0))
		return (1);
	return (0);
}

void	release_forks(t_list_item *philo)
{
	pthread_mutex_lock(&philo->next->mutex);
	if (philo->next->fork == 1)
		philo->next->fork = 0;
	pthread_mutex_unlock(&philo->next->mutex);
	if (philo->prev)
	{
		pthread_mutex_lock(&philo->prev->mutex);
		if (philo->prev->fork == 1)
			philo->prev->fork = 0;
		pthread_mutex_unlock(&philo->prev->mutex);
	}
}

/* fonction that comnpares th current time with the last meal time and 
** if it is greater than the time to die, it kills the philosopher*/
int	check_starved(t_list_item *philo)
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
		if (printf_mutex(philo, 4))
			return (1);
		return (1);
	}
	return (0);
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
