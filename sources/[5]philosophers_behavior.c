/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   [5]philosophers_behavior.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:06:37 by motero            #+#    #+#             */
/*   Updated: 2022/12/08 23:48:24 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// A function that runs in a separate thread for each philosopher
void	*philosopher_thread(void *arg)
{
	t_thread_info	*info;
	t_list_item		*philosopher;
	t_arguments		args;
	struct timeval	start;
	int				time;
	int				time_loop;

	info = (t_thread_info *)arg;
	philosopher = info->item;
	args = (t_arguments)philosopher->args;
	gettimeofday(&start, NULL);
	while (1)
	{
		ft_philo_starved(start, info);
		if (philosopher->state == SLEEPING)
			time_loop = philosopher->args.time_to_sleep;
		else if (philosopher->state == THINKING)
			time_loop = philosopher->args.time_to_die;
		while (philosopher->state != HUNGRY)
		{
			ft_display_status(start, info);
			time = 0;
			while (time < time_loop)
			{
				ft_philo_starved(start, info);
				usleep(1000);
				time += 1;
			}
			philosopher->state += 1;
		}
		if (philosopher->state == HUNGRY)
		{
			ft_try_eat(start, info);
			time_loop = philosopher->args.time_to_eat;
			ft_display_status(start, info);
			time = 0;
			while (time < time_loop)
			{
				ft_philo_starved(start, info);
				usleep(1000);
				time += 1;
			}
			ft_put_down_forks(philosopher);
			ft_philo_starved(start, info);
			philosopher->state = SLEEPING;
		}
	}
}

// Function to display the current state of a philosopher
void	ft_display_status(struct timeval start, t_thread_info *info)
{
	struct timeval	current;
	t_list_item		*philosopher;
	int				timestamp;
	int				i;

	pthread_mutex_lock(info->display_mutex);
	philosopher = info->item;
	gettimeofday(&current, NULL);
	timestamp = (current.tv_sec - start.tv_sec) * 1000
		+ (current.tv_usec - start.tv_usec) / 1000;
	i = philosopher->number;
	if (philosopher->state == THINKING)
		ft_printf("%d ms Philosopher %d is thinking\n", timestamp, i);
	else if (philosopher->state == FORKING)
		ft_printf("%d ms Philosopher %d has taken a fork\n", timestamp, i);
	else if (philosopher->state == EATING)
		ft_printf("%d ms Philosopher %d is eating\n", timestamp, i);
	else if (philosopher->state == SLEEPING)
		ft_printf("%d ms Philosopher %d is sleeping\n", timestamp, i);
	else if (philosopher->state == DEAD)
		ft_printf("%d ms Philosopher %d died\n", timestamp, i);
	else
		ft_printf("ERROR: Philosopher %d has an invalid state\n", i);
	pthread_mutex_unlock(info->display_mutex);
}

// Function to try to pick up the forks on either side of a philosopher
int	ft_try_eat(struct timeval start, t_thread_info *info)
{
	t_list_item	*left_fork;
	t_list_item	*right_fork;
	t_list_item	*philosopher;

	philosopher = info->item;
	left_fork = philosopher->prev;
	right_fork = philosopher->next;
	philosopher->state = FORKING;
	pthread_mutex_lock(&left_fork->mutex);
	ft_display_status(start, info);
	pthread_mutex_lock(&right_fork->mutex);
	ft_display_status(start, info);
	usleep(500);
	philosopher->state = EATING;
	return (1);
}

// Function to put down the forks after a philosopher is done eating
void	ft_put_down_forks(t_list_item *philosopher)
{
	t_list_item	*left_fork;
	t_list_item	*right_fork;

	left_fork = philosopher->prev;
	right_fork = philosopher->next;
	pthread_mutex_unlock(&left_fork->mutex);
	pthread_mutex_unlock(&right_fork->mutex);
}

void	ft_philo_starved(struct timeval start, t_thread_info *info)
{
	int				elapsed_time;
	t_list_item		*philosopher;
	struct timeval	current;

	philosopher = info->item;
	gettimeofday(&current, NULL);
	elapsed_time = (current.tv_sec - start.tv_sec) * 1000
		+ (current.tv_usec - start.tv_usec) / 1000;
	if (elapsed_time >= philosopher->args.time_to_die)
	{
		philosopher->state = DEAD;
		ft_display_status(start, info);
		pthread_exit(0);
	}
}
