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

void	printf_mutex(t_list_item *philo, int state);
void	grab_forks(t_list_item *philo);
void	release_forks(t_list_item *philo);
void	safe_sleep(t_list_item *philo, long int time_to_sleep);

void	*philo_thread(void *arg)
{
	t_list_item		*philo;
	t_thread_info	*info;

	philo = (t_list_item *) arg;
	info = philo->info;
	while (1)
	{
		pthread_mutex_lock(info->death_mutex);
		if (info->end == 1)
			break ;
		pthread_mutex_unlock(info->death_mutex);
		if (philo->state == SLEEPING)
		{
			printf_mutex(philo, 2);
			//ft_usleep(philo->args.time_to_sleep);
			safe_sleep(philo, philo->args.time_to_sleep);
			philo->state++;
		}
		if (philo->state == THINKING)
		{
			printf_mutex(philo, 3);
			philo->state--;
		}
		grab_forks(philo);
		gettimeofday(&philo->timestamps->start_last_meal, NULL);
		printf_mutex(philo, 1);
		//ft_usleep(philo->args.time_to_eat);
		safe_sleep(philo, philo->args.time_to_eat);
		release_forks(philo);
	}
	pthread_exit(0);
}

//a function tha splits the sleep function into smaller ones so we can check if he died of starvation or if another philospher die
void	safe_sleep(t_list_item *philo, long int time_to_sleep)
{
	long int				time_in_ms;
	struct timeval			current;
	struct timeval const	last_meal = philo->timestamps->start_last_meal;

	gettimeofday(&current, NULL);
	time_in_ms = (current.tv_sec - last_meal.tv_sec) * 1000
		+ (current.tv_usec - last_meal.tv_usec) / 1000;
	while (time_in_ms < time_to_sleep)
	{
		ft_usleep(1);
		time_in_ms += 5;
		pthread_mutex_lock(philo->info->death_mutex);
		if (philo->info->end == 1)
		{
			pthread_mutex_unlock(philo->info->death_mutex);
			pthread_exit(0);
		}
		pthread_mutex_unlock(philo->info->death_mutex);
		gettimeofday(&current, NULL);
		time_in_ms = (current.tv_sec - last_meal.tv_sec) * 1000
			+ (current.tv_usec - last_meal.tv_usec) / 1000;
		if (time_in_ms >= philo->args.time_to_die)
		{
			printf_mutex(philo, 4);
			pthread_mutex_lock(philo->info->death_mutex);
			philo->info->end = 1;
			pthread_mutex_unlock(philo->info->death_mutex);
			pthread_exit(0);
		}
	}
}

void	printf_mutex(t_list_item *philo, int state)
{
	struct timeval	current;
	t_thread_info	*info;
	int				timestamp;
	int	const		i = philo->number;

	info = philo->info;
	pthread_mutex_lock(info->display_mutex);
	gettimeofday(&current, NULL);
	philo->timestamps->current = current;
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
		ft_printf("%d %d died\n", timestamp, i);
	pthread_mutex_unlock(info->display_mutex);
}

void	grab_forks(t_list_item *philo)
{
	if (philo->args.number_of_philosophers % 2 == 0)
	{
		pthread_mutex_lock(&philo->prev->mutex);
		printf_mutex(philo, 0);
		pthread_mutex_lock(&philo->next->mutex);
		printf_mutex(philo, 0);
	}
	else
	{
		if (philo->number % 2 == 0)
		{
			pthread_mutex_lock(&philo->prev->mutex);
			printf_mutex(philo, 0);
			pthread_mutex_lock(&philo->next->mutex);
			printf_mutex(philo, 0);
		}
		else
		{
			pthread_mutex_lock(&philo->next->mutex);
			printf_mutex(philo, 0);
			pthread_mutex_lock(&philo->prev->mutex);
			printf_mutex(philo, 0);
		}
	}
}

void	release_forks(t_list_item *philo)
{
	if (philo->args.number_of_philosophers % 2 == 0)
	{
		pthread_mutex_unlock(&philo->next->mutex);
		pthread_mutex_unlock(&philo->prev->mutex);
	}
	else
	{
		if (philo->number % 2 == 0)
		{
			pthread_mutex_unlock(&philo->prev->mutex);
			pthread_mutex_unlock(&philo->next->mutex);
		}
		else
		{
			pthread_mutex_unlock(&philo->next->mutex);
			pthread_mutex_unlock(&philo->prev->mutex);
		}
	}
}
// void	*philo_thread(void *arg)
// {
// 	t_list_item		*philosopher;

// 	philosopher = (t_list_item *) arg;
// 	while (!ft_stop_signal(philosopher))
// 	{
// 		if (philosopher->state == SLEEPING)
// 		{
// 			ft_display_status(philosopher->timestamps->start, philosopher, 0);
// 			ft_usleep(philosopher->args.time_to_sleep);
// 			philosopher->state = THINKING;
// 			continue ;
// 		}
// 		if (philosopher->state == THINKING)
// 		{
// 			ft_display_status(philosopher->timestamps->start, philosopher, 0);
// 			philosopher->state = HUNGRY;
// 			continue ;
// 		}
// 		if (philosopher->state == HUNGRY)
// 		{
// 			if (!ft_try_eat(philosopher->timestamps->start, philosopher))
// 			{
// 				philosopher->state = END;
// 				continue ;
// 			}
// 			ft_display_status(philosopher->timestamps->start, philosopher, 0);
// 			ft_usleep(philosopher->args.time_to_eat);
// 			ft_put_down_forks(philosopher);
// 			philosopher->state = SLEEPING;
// 		}
// 		if (philosopher->state == DEAD)
// 			break ;
// 		if (philosopher->state == END)
// 		{
// 			pthread_exit(0);
// 		}
// 	}
// 	pthread_exit(0);
// }

// Function to display the current state of a philosopher
void	ft_display_status(struct timeval start, t_list_item *philo, int fork)
{
	struct timeval	current;
	t_thread_info	*info;
	int				timestamp;
	int				i;

	info = philo->info;
	pthread_mutex_lock(info->display_mutex);
	gettimeofday(&current, NULL);
	philo->timestamps->current = current;
	timestamp = (current.tv_sec - start.tv_sec) * 1000
		+ (current.tv_usec - start.tv_usec) / 1000;
	i = philo->number;
	if (fork == 3)
		ft_printf("%d %d died\n", timestamp, i);
	else if (philo->state == THINKING)
		ft_printf("%d %d is thinking\n", timestamp, i);
	else if (philo->state == FORKING)
	{
		if (fork == 0)
		{
			ft_printf("%d %d has taken a fork\n", timestamp, i);
		}
		else
			ft_printf("%d %d has taken a fork\n", timestamp, i);
	}
	else if (philo->state == EATING)
		ft_printf("%d %d is eating\n", timestamp, i);
	else if (philo->state == SLEEPING)
		ft_printf("%d %d is sleeping\n", timestamp, i);
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
	if (left_fork)
		ft_display_status(start, philo, 0);
	if (right_fork)
		ft_display_status(start, philo, 1);
	if (left_fork && right_fork)
		philo->state = EATING;
	if (philo->state != EATING)
	{
		return (0);
	}
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
	{
		pthread_mutex_lock(info->death_mutex);
		info->nbr_philo_full += 1;
		pthread_mutex_unlock(info->death_mutex);
	}
}

int	ft_philo_starved(struct timeval start, t_list_item *philo)
{
	int				start_last_meal;
	t_thread_info	*info;
	struct timeval	current;

	info = philo->info;
	gettimeofday(&current, NULL);
	start_last_meal = (current.tv_sec - philo->timestamps->start_last_meal.tv_sec) * 1000
		+ (current.tv_usec - philo->timestamps->start_last_meal.tv_usec) / 1000;
	if (start_last_meal >= philo->args.time_to_die)
	{
		pthread_mutex_lock(info->death_mutex);
		info->someone_died += 1;
		pthread_mutex_unlock(info->death_mutex);
		ft_display_status(start, philo, 3);
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
	if (args.number_of_times_each_philosopher_must_eat >= 0)
	{
		if (args.number_of_times_each_philosopher_must_eat == 0)
		{
			pthread_mutex_lock(info->death_mutex);
			info->nbr_philo_full = philo->args.number_of_philosophers;
			pthread_mutex_unlock(info->death_mutex);
		}
		pthread_mutex_lock(info->death_mutex);
		if (info->nbr_philo_full >= args.number_of_philosophers)
		{
			pthread_mutex_unlock(info->death_mutex);
			if (philo->left_fork || philo->right_fork)
				ft_put_down_forks(philo);
			pthread_exit(0);
		}
		pthread_mutex_unlock(info->death_mutex);

	}
	pthread_mutex_lock(info->death_mutex);
	if (info->someone_died >= 1)
	{
	pthread_mutex_unlock(info->death_mutex);
		if (philo->state == FORKING)
			ft_put_down_forks(philo);
		pthread_exit(0);
	}
	pthread_mutex_unlock(info->death_mutex);
	if (ft_philo_starved(philo->timestamps->start, philo))
	{
		if (philo->state == FORKING)
			ft_put_down_forks(philo);
		pthread_exit(0);
	}
	return (0);
}

//createown usleep with  gettimeofday function
void	ft_usleep(long int time_in_ms)
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
		usleep(500);
	}
}
