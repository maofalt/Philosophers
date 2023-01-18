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
	t_state	state;

	state = ft_decide_state(PHILOSOPHER, *args, i);
	*philosopher = create_item(PHILOSOPHER, state, *args, i);
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
	item->args = a;
	item->type = type;
	item->number = nb;
	item->times_eaten = 0;
	item->timestamps = NULL;
	if (ft_philo_allocation(item, type, state) == 0)
		return (NULL);
	else if (type == FORK)
		pthread_mutex_init(&item->mutex, NULL);
	item->prev = NULL;
	item->next = NULL;
	item->info = NULL;
	return (item);
}

int	ft_philo_allocation(t_list_item *item, t_item_type type, t_state state)
{
	if (type == PHILOSOPHER)
	{
		item->thread = malloc(sizeof(pthread_t));
		if (item->thread == NULL)
			return (free(item), 0);
		item->state = state;
		item->timestamps = ft_calloc(1, sizeof(t_timestamps));
		if (item->timestamps == NULL)
		{
			free(item->thread);
			return (free(item), 0);
		}
	}
	return (1);
}

// A helper function that decides the state of a philosopher
/* if total_nbr_philo is even, even philosopher start with THINKING state
** if total_nbr_philo is odd, odd philosopher start with THINKING state
** if total_nbr_philo is even, odd philosopher start with SLEEPING state
** if total_nbr_philo is odd, even philosopher start with SLEEPING state
*/
t_state	ft_decide_state(t_item_type type, t_arguments a, int nb)
{
	if (type == PHILOSOPHER)
	{
		if (a.number_of_philosophers % 2 == 0)
		{
			if (nb % 2 == 0)
				return (THINKING);
			else
				return (SLEEPING);
		}
		else
		{
			if (nb % 2 == 0)
				return (SLEEPING);
			else
				return (THINKING);
		}
	}
	return (0);
}
