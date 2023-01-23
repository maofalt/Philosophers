/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   [0]main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:06:37 by motero            #+#    #+#             */
/*   Updated: 2022/12/08 22:04:47 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_arguments		*args;
	t_list_item		*list;
	t_thread_info	*info;

	if (!ft_valid_arguments(argc, argv))
	{
		ft_printf("Invalid arguments\n");
		return (0);
	}
	args = ft_parse_arguments(argc, argv);
	if (!args)
		return (0);
	list = ft_create_list(args);
	if (!list)
		return (free(args), 0);
	if (!ft_create_threads(list))
	{
		info = list->info;
		ft_wait_threads(list);
		free_info(info);
	}
	free_list(list);
	free(args);
}

void	ft_wait_threads(t_list_item *list)
{
	t_list_item	*item;
	int			i;
	int			max;

	item = list;
	list = list->next;
	i = 0;
	max = item->args.number_of_philosophers * 2;
	while (i++ < max)
	{
		if (item->type == PHILOSOPHER)
		{
			if (pthread_join(*item->thread, NULL))
				printf ("Erroir when trying to join thread\n");
		}
		if (item->next)
			item = item->next;
	}
}

void	death_message(t_list_item *philo, int ts)
{
	ft_printf("%d %d died\n", ts, philo->number);
	philo->info->end += 1;
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
