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
