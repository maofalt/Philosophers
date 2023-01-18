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
	t_list_item		*philosopher;

	philosopher = (t_list_item *) arg;
	while (!ft_stop_signal(philosopher))
	{
		if (philosopher->state == SLEEPING)
		{
			ft_display_status(philosopher->timestamps->start, philosopher);
			usleep(philosopher->args.time_to_sleep * 1000);
			philosopher->state = THINKING;
			continue ;
		}
		if (philosopher->state == THINKING)
		{
			ft_display_status(philosopher->timestamps->start, philosopher);
			philosopher->state = HUNGRY;
			continue ;
		}
		if (philosopher->state == HUNGRY)
		{
			if (!ft_try_eat(philosopher->timestamps->start, philosopher))
			{
				continue ;
			}
			ft_display_status(philosopher->timestamps->start, philosopher);
			usleep(philosopher->args.time_to_eat * 1000);
			ft_put_down_forks(philosopher);
			philosopher->state = SLEEPING;
		}
		if (philosopher->state == DEAD)
			break ;
		if (philosopher->state == END)
			pthread_exit(0);
	}
	pthread_exit(0);
}

// Function to display the current state of a philosopher
void	ft_display_status(struct timeval start, t_list_item *philo)
{
	struct timeval	current;
	t_thread_info	*info;
	int				timestamp;
	int				i;

	info = philo->info;
	gettimeofday(&current, NULL);
	pthread_mutex_lock(info->display_mutex);
	philo->timestamps->current = current;
	timestamp = (current.tv_sec - start.tv_sec) * 1000
		+ (current.tv_usec - start.tv_usec) / 1000;
	i = philo->number;
	if (philo->state == THINKING)
		ft_printf("%d %d is thinking\n", timestamp, i);
	else if (philo->state == FORKING)
		ft_printf("%d %d has taken a fork\n", timestamp, i);
	else if (philo->state == EATING)
		ft_printf("%d %d is eating\n", timestamp, i);
	else if (philo->state == SLEEPING)
		ft_printf("%d %d is sleeping\n", timestamp, i);
	else if (philo->state == DEAD)
		ft_printf("%d %d died\n", timestamp, i);
	else if (philo->state > END)
		ft_printf("ERROR: Philosopher %d has an invalid state\n", i);
	pthread_mutex_unlock(info->display_mutex);
}

// Function to try to pick up the forks on either side of a philosopher
int	ft_try_eat(struct timeval start, t_list_item *philo)
{
	t_list_item		*left_fork;
	t_list_item		*right_fork;
	struct timeval	current;

	left_fork = NULL;
	right_fork = NULL;
	if (philo->prev != NULL)
		left_fork = philo->prev;
	if (philo->next != NULL)
		right_fork = philo->next;
	philo->state = FORKING;
	if (left_fork && !ft_stop_signal(philo))
	{
		pthread_mutex_lock(&left_fork->mutex);
		ft_display_status(start, philo);
	}
	if (right_fork && !ft_stop_signal(philo))
	{
		pthread_mutex_lock(&right_fork->mutex);
		ft_display_status(start, philo);
	}
	if (left_fork && right_fork && !ft_stop_signal(philo))
		philo->state = EATING;
	if (philo->state != EATING)
		return (0);
	gettimeofday(&current, NULL);
	philo->timestamps->start_last_meal = current;
	return (1);
}

// Function to put down the forks after a philosopher is done eating
void	ft_put_down_forks(t_list_item *philo)
{
	t_list_item				*left_fork;
	t_list_item				*right_fork;
	t_thread_info			*info;
	struct timeval			current;

	info = philo->info;
	left_fork = philo->prev;
	right_fork = philo->next;
	if (left_fork)
		pthread_mutex_unlock(&left_fork->mutex);
	if (right_fork)
		pthread_mutex_unlock(&right_fork->mutex);
	gettimeofday(&current, NULL);
	philo->timestamps->delta_last_meal = (current.tv_sec - philo->timestamps->start_last_meal.tv_sec) * 1000
		+ (current.tv_usec - philo->timestamps->start_last_meal.tv_usec) / 1000;
	philo->times_eaten++;
	if (philo->times_eaten == philo->args.number_of_times_each_philosopher_must_eat)
		info->nbr_philo_full += 1;
}

int	ft_philo_starved(struct timeval start, t_list_item *philo)
{
	int				start_last_meal;
	t_thread_info	*info;
	struct timeval	current;

	info = philo->info;
	gettimeofday(&current, NULL);
	philo->timestamps->delta_last_meal = (current.tv_sec - philo->timestamps->start_last_meal.tv_sec) * 1000
		+ (current.tv_usec - philo->timestamps->start_last_meal.tv_usec) / 1000;
	start_last_meal = philo->timestamps->delta_last_meal;
	if (start_last_meal >= philo->args.time_to_die && philo->state != EATING)
	{
		philo->state = DEAD;
		pthread_mutex_lock(info->death_mutex);
			info->someone_died += 1;
		if (info->someone_died == 1)
			ft_display_status(start, philo);
		pthread_mutex_unlock(info->death_mutex);
		return (1);
	}
	return (0);
}

int	ft_stop_signal(t_list_item *philo)
{
	t_arguments		args;
	t_thread_info	*info;

	info = philo->info;
	args = philo->args;
	if (args.number_of_times_each_philosopher_must_eat > 0)
	{
		if (info->nbr_philo_full >= philo->args.number_of_philosophers)
		{
			ft_put_down_forks(philo);
			pthread_exit(0);
		}
	}
	pthread_mutex_lock(info->death_mutex);
	if (info->someone_died >= 1)
	{
		pthread_mutex_unlock(info->death_mutex);
		ft_put_down_forks(philo);
		pthread_exit(0);
	}
	pthread_mutex_unlock(info->death_mutex);
	if (info->nbr_philo_full >= args.number_of_philosophers)
	{
		ft_put_down_forks(philo);
		pthread_exit(0);
	}
	if (ft_philo_starved(philo->timestamps->start, philo))
	{
		ft_put_down_forks(philo);
		pthread_exit(0);
	}
	return (0);
}
