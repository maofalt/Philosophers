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

	if (!ft_init_first_philo_fork(&first, &fork, args))
		return (NULL);
	prev = fork;
	i = 2;
	while (args->number_of_philosophers > 1
		&& i <= args->number_of_philosophers)
	{
		if (!ft_create_philo(&philosopher, args, i, &first))
			return (NULL);
		link_items(prev, philosopher);
		if (!ft_create_fork(&fork, args, i++, &first))
			return (NULL);
		link_items(philosopher, fork);
		prev = fork;
	}
	return (link_items(prev, first), first);
}

int	ft_init_first_philo_fork(t_list_item **first, t_list_item **fork,
t_arguments *args)
{
	*first = create_item(PHILOSOPHER, HUNGRY, *args, 1);
	if (*first == NULL)
		return (0);
	*fork = create_item(FORK, 0, *args, 1);
	if (*fork == NULL)
	{
		free(*first);
		return (0);
	}
	link_items(*first, *fork);
	return (1);
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

/* A function that frees the memory used by the linked
list of philosopherand forks*/
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
