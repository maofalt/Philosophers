/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   [2]parse_arguments.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:06:37 by motero            #+#    #+#             */
/*   Updated: 2023/01/16 22:48:43 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// A function that parses the command line arguments
t_arguments	*ft_parse_arguments(int argc, char **argv)
{
	t_arguments	*args;

	args = malloc(sizeof(t_arguments));
	if (!args)
		return (NULL);
	ft_memset(args, 0, sizeof(t_arguments));
	args->number_of_philosophers = ft_atoi(argv[1]);
	args->time_to_die = ft_atoi(argv[2]);
	args->time_to_eat = ft_atoi(argv[3]);
	args->time_to_sleep = ft_atoi(argv[4]);
	args->number_of_times_each_philosopher_must_eat = -1;
	if (argc == 6)
		args->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	return (args);
}
