/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   [4]threads_manipulation.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:06:37 by motero            #+#    #+#             */
/*   Updated: 2022/12/07 22:21:08 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_create_threads(t_arguments args, t_list_item *list)
{
	pthread_t	*philosopher;
	int			i;

	(void)list;
	philosopher = malloc(sizeof(pthread_t) * args.number_of_philosophers);
	if (!philosopher)
		return ;
	i = 0;
	while (i < args.number_of_philosophers)
	{
		//pthread_create(&philosopher[i], NULL, philosopher_behavior, get_philosopher(list, i + 1));
		i++;
	}
}
