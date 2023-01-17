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

// A function that creates the linked list of philosophers and forks
t_list_item	*ft_create_list(t_arguments *args)
{
	t_list_item	*first;
	t_list_item	*fork;
	t_list_item	*prev;
	t_list_item	*philosopher;
	int			i;

	if (!ft_init_first_philo_fork(first, fork, args))
		return (NULL);
	prev = fork;
	if (args->number_of_philosophers == 1)
		return (first);
	i = 2;
	while (i <= args->number_of_philosophers)
	{
		if (!ft_create_philo(philosopher, args, i, first))
			return (NULL);
		link_items(prev, philosopher);
		if (!ft_create_fork(fork, args, i, first))
			return (NULL);
		link_items(philosopher, fork);
		prev = fork;
		i++;
	}
	link_items(prev, first);
	return (first);
}

int	ft_init_first_philo_fork(t_list_item *first, t_list_item *fork,
t_arguments *args)
{
	first = create_item(PHILOSOPHER, HUNGRY, *args, 1);
	if (first == NULL)
		return (0);
	fork = create_item(FORK, 0, *args, 1);
	if (fork == NULL)
	{
		free(first);
		return (0);
	}
	link_items(first, fork);
	return (1);
}

int	ft_create_philo(t_list_item *philosopher, t_arguments *args, int i,
t_list_item *first)
{
	philosopher = create_item(PHILOSOPHER, HUNGRY, *args, i);
	if (philosopher == NULL)
	{
		free_list(first);
		return (0);
	}
	return (1);
}

int	ft_create_fork(t_list_item *fork, t_arguments *args, int i,
t_list_item *first)
{
	fork = create_item(FORK, 0, *args, i);
	if (fork == NULL)
	{
		free_list(first);
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
	if (type == PHILOSOPHER)
	{
		item->thread = malloc(sizeof(pthread_t));
		if (item->thread == NULL)
		{
			free(item);
			return (NULL);
		}
		item->state = THINKING;
	}
	if (type == PHILOSOPHER && nb % 2 == 0)
		item->state = THINKING;
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
		free(item);
		return (NULL);
	}
	return (item);
}

// A helper function that links two items together in a double-linked list
void	link_items(t_list_item *prev, t_list_item *next)
{
	prev->next = next;
	next->prev = prev;
}

// A function that prints the linked list of philosophers and forks
void	ft_print_list(t_list_item *first)
{
	t_list_item	*item;

	ft_printf("%d: ", first->number);
	if (first->type == PHILOSOPHER)
		ft_printf("PHILOSOPHER");
	else
		ft_printf("FORK");
	ft_printf("\n");
	item = first->next;
	while (item != first)
	{
		ft_printf("%d: ", item->number);
		if (item->type == PHILOSOPHER)
			ft_printf("PHILOSOPHER");
		else
			ft_printf("FORK");
		ft_printf("\n");
		item = item->next;
	}
}

// A function that frees the memory used by the linked list of philosophers and forks
void	free_list(t_list_item *first)
{
	t_list_item	*item;
	t_list_item	*prev;
	int			i;
	int			max;	

	item = first;
	first = first->next;
	i = 0;
	max = item->args.number_of_philosophers;
	while (i++ < max)
	{
		if (item->type == PHILOSOPHER)
		{
			pthread_join(*item->thread, NULL);
			free(item->thread);
		}
		else
			pthread_mutex_destroy(&item->mutex);
		prev = item;
		item = item->next;
		free(prev);
	}
}
