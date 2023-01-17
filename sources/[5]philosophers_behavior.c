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

void	*philosopher_thread(void *arg)
{
	t_thread_info	*info;
	t_list_item		*philosopher;
	//int				time;
	//int				time_loop;

	info = (t_thread_info *)arg;
	philosopher = info->item;
	while (!ft_stop_signal(info))
	{
		// if (philosopher->state == SLEEPING)
		// 	time_loop = philosopher->args.time_to_sleep * 10;
		// else if (philosopher->state == THINKING)
		// 	time_loop = philosopher->args.time_to_die * 10;
		if (philosopher->state == SLEEPING)
		{
			ft_display_status(info->timestamps->start, info);
			usleep(philosopher->args.time_to_sleep * 1000);
			// while (time++ < time_loop)
			// 	usleep(1000);
			philosopher->state += 1;
		}
		if (philosopher->state == THINKING)
		{
			ft_display_status(info->timestamps->start, info);
			philosopher->state += 1;
		}
		if (philosopher->state == HUNGRY)
		{
			if (!ft_try_eat(info->timestamps->start, info))
			{
				while (!ft_stop_signal(info))
					usleep(500);
			}
			ft_display_status(info->timestamps->start, info);
			// time_loop = philosopher->args.time_to_eat;
			// time = 0;
			usleep(philosopher->args.time_to_eat * 1000);
			// while (time++ < time_loop)
			// 	usleep(1000);
			ft_put_down_forks(info);
			philosopher->state = SLEEPING;
		}
		if (philosopher->state == DEAD)
			break ;
		if (philosopher->state == END)
			pthread_exit(0);
	}
	pthread_exit(0);
}

// void	*philosopher_thread(void *arg)
// {
// 	t_thread_info	*info;
// 	t_list_item		*philosopher;
// 	t_arguments		args;
// 	int				time;
// 	int				time_loop;

// 	info = (t_thread_info *)arg;
// 	philosopher = info->item;
// 	args = (t_arguments)philosopher->args;
// 	while (!ft_stop_signal(info))
// 	{
// 		if (philosopher->state == SLEEPING)
// 			time_loop = philosopher->args.time_to_sleep * 10;
// 		else if (philosopher->state == THINKING)
// 			time_loop = philosopher->args.time_to_die * 10;
// 		if (philosopher->state == SLEEPING)
// 		{
// 			ft_display_status(info->timestamps->start, info);
// 			time = 0;
// 			while (time < time_loop && !ft_stop_signal(info))
// 			{
// 				usleep(500);
// 				time += 1;
// 			}
// 			philosopher->state += 1;
// 		}
// 		if (philosopher->state == THINKING && !ft_stop_signal(info))
// 		{
// 			ft_display_status(info->timestamps->start, info);
// 			philosopher->state += 1;
// 		}
// 		if (philosopher->state == HUNGRY &&!ft_stop_signal(info))
// 		{
// 			ft_try_eat(info->timestamps->start, info);
// 			time_loop = philosopher->args.time_to_eat;
// 			ft_display_status(info->timestamps->start, info);
// 			time = 0;
// 			while (time < time_loop && !ft_stop_signal(info))
// 			{
// 				usleep(500);
// 				time += 1;
// 			}
// 			ft_put_down_forks(info);
// 			ft_stop_signal(info);
// 			philosopher->state = SLEEPING;
// 		}
// 	}
// 	return (NULL);
// }

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
	info->timestamps->current = current;
	timestamp = (current.tv_sec - start.tv_sec) * 1000
		+ (current.tv_usec - start.tv_usec) / 1000;
	i = philosopher->number;
	if (philosopher->state == THINKING)
		ft_printf("%d %d is thinking\n", timestamp, i);
	else if (philosopher->state == FORKING)
		ft_printf("%d %d has taken a fork\n", timestamp, i);
	else if (philosopher->state == EATING)
		ft_printf("%d %d is eating\n", timestamp, i);
	else if (philosopher->state == SLEEPING)
		ft_printf("%d %d is sleeping\n", timestamp, i);
	else if (philosopher->state == DEAD)
		ft_printf("%d %d died\n", timestamp, i);
	else
		ft_printf("ERROR: Philosopher %d has an invalid state\n", i);
	pthread_mutex_unlock(info->display_mutex);
}

// Function to try to pick up the forks on either side of a philosopher
int	ft_try_eat(struct timeval start, t_thread_info *info)
{
	t_list_item		*left_fork;
	t_list_item		*right_fork;
	t_list_item		*philosopher;
	struct timeval	current;

	philosopher = info->item;
	left_fork = NULL;
	right_fork = NULL;
	if (philosopher->prev != NULL)
		left_fork = philosopher->prev;
	if (philosopher->next != NULL)
		right_fork = philosopher->next;
	if (philosopher->number % 2 == 0)
	{
		philosopher->state = FORKING;
		if (right_fork)
		{
			pthread_mutex_lock(&right_fork->mutex);
			ft_display_status(start, info);
		}
		if (left_fork)
		{
			pthread_mutex_lock(&left_fork->mutex);
			ft_display_status(start, info);
		}
		philosopher->state = EATING;
	}
	else
	{
		philosopher->state = FORKING;
		if (left_fork)
		{
			pthread_mutex_lock(&left_fork->mutex);
			ft_display_status(start, info);
		}
		if (right_fork)
		{
			pthread_mutex_lock(&right_fork->mutex);
			ft_display_status(start, info);
		}
		if (left_fork && right_fork)
			philosopher->state = EATING;
	}
	if (philosopher->state != EATING)
		return (0);
	gettimeofday(&current, NULL);
	info->timestamps->start_last_meal = current;
	return (1);
}


// Function to put down the forks after a philosopher is done eating
void	ft_put_down_forks(t_thread_info *info)
{
	t_list_item				*left_fork;
	t_list_item				*right_fork;
	t_list_item				*philosopher;
	struct timeval			current;

	philosopher = info->item;
	left_fork = philosopher->prev;
	right_fork = philosopher->next;
	if (left_fork)
		pthread_mutex_unlock(&left_fork->mutex);
	if (right_fork)
		pthread_mutex_unlock(&right_fork->mutex);
	gettimeofday(&current, NULL);
	info->timestamps->delta_last_meal = (current.tv_sec - info->timestamps->start_last_meal.tv_sec) * 1000
		+ (current.tv_usec - info->timestamps->start_last_meal.tv_usec) / 1000;
	info->times_eaten++;
	if (info->times_eaten == philosopher->args.number_of_times_each_philosopher_must_eat)
		*info->nbr_philo_full += 1;
}

int	ft_philo_starved(struct timeval start, t_thread_info *info)
{
	int				start_last_meal;
	t_list_item		*philosopher;
	struct timeval	current;

	philosopher = info->item;
	gettimeofday(&current, NULL);
	//printf("philosopher %d\n", philosopher->number);
	info->timestamps->delta_last_meal = (current.tv_sec - info->timestamps->start_last_meal.tv_sec) * 1000
		+ (current.tv_usec - info->timestamps->start_last_meal.tv_usec) / 1000;
	start_last_meal = info->timestamps->delta_last_meal;
	if (philosopher->number == 1)
		printf("time since last meal: %d\n", start_last_meal);
	if (start_last_meal >= philosopher->args.time_to_die && philosopher->state != EATING)
	{
		philosopher->state = DEAD;
		pthread_mutex_lock(info->death_mutex);
			*info->someone_died += 1;
		if (*info->someone_died == 1)
			ft_display_status(start, info);
		pthread_mutex_unlock(info->death_mutex);
		return (1);
	}
	return (0);
}

int	ft_stop_signal(t_thread_info *info)
{
	t_arguments	args;
	t_list_item	*philosopher;

	philosopher = info->item;
	args = philosopher->args;
	if (args.number_of_times_each_philosopher_must_eat > 0)
	{
		if (*info->nbr_philo_full >= philosopher->args.number_of_philosophers)
		{
			// pthread_mutex_lock(info->display_mutex);
			// pthread_mutex_unlock(info->display_mutex);
			ft_put_down_forks(info);
			pthread_exit(0);
		}
	}
	if (*info->someone_died >= 1)
	{
		ft_put_down_forks(info);
		pthread_exit(0);
	}
	if (*info->nbr_philo_full >= args.number_of_philosophers)
	{
		ft_put_down_forks(info);
		pthread_exit(0);
	}
	if (ft_philo_starved(info->timestamps->start, info))
	{
		ft_put_down_forks(info);
		pthread_exit(0);
	}
	return (0);
}
