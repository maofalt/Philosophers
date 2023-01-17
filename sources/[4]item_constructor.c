/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   [3]list_manipulation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:06:37 by motero            #+#    #+#             */
/*   Updated: 2023/01/16 23:22:58 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_create_philo(t_list_item **philosopher, t_arguments *args, int i,
t_list_item **first)
{
	*philosopher = create_item(PHILOSOPHER, HUNGRY, *args, i);
	if (*philosopher == NULL)
	{
		free_list(*first);
		return (0);
	}
	return (1);
}

int	ft_create_fork(t_list_item **fork, t_arguments *args, int i,
t_list_item **first)
{
	*fork = create_item(FORK, 0, *args, i);
	if (fork == NULL)
	{
		free_list(*first);
		return (0);
	}
	return (1);
}

// A helper function that creates a philosopher  or a fork with a given number
t_list_item	*create_item(t_item_type type, t_state state, t_arguments a, int nb)
{
	t_list_item			*item;

	item = malloc(sizeof(t_list_item));
	if (!item)
		return (NULL);
	(void)state;
	item->args = a;
	item->type = type;
	item->number = nb;
	if (ft_philo_allocation(item, type, nb) == 0)
		return (NULL);
	else if (type == FORK)
		pthread_mutex_init(&item->mutex, NULL);
	item->prev = NULL;
	item->next = NULL;
	ft_create_info(item);
	if (item->info == NULL)
	{
		if (type == FORK)
			pthread_mutex_destroy(&item->mutex);
		free(item->thread);
		return (free(item), NULL);
	}
	return (item);
}

int	ft_philo_allocation(t_list_item *item, t_item_type type, int nb)
{
	if (type == PHILOSOPHER)
	{
		item->thread = malloc(sizeof(pthread_t));
		if (item->thread == NULL)
			return (free(item), 0);
		item->state = THINKING;
	}
	if (type == PHILOSOPHER && nb % 2 == 0)
		item->state = THINKING;
	return (1);
}
