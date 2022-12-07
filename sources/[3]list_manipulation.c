/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   [3]list_manipulation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:06:37 by motero            #+#    #+#             */
/*   Updated: 2022/12/07 22:55:21 by motero           ###   ########.fr       */
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

	first = create_item(PHILOSOPHER, THINKING, 1);
	fork = create_item(FORK, NULL, 1);
	link_items(first, fork);
	prev = fork;
	i = 2;
	while (i <= args->number_of_philosophers)
	{
		philosopher = create_item(PHILOSOPHER, THINKING, i);
		link_items(prev, philosopher);
		fork = create_item(FORK, NULL, i);
		link_items(philosopher, fork);
		prev = fork;
		i++;
	}
	link_items(prev, first);
	return (first);
}

// A helper function that creates a philosopher  or a fork with a given number
t_list_item	*create_item(t_item_type type, t_state state, int number)
{
	t_list_item	*item;

	item = malloc(sizeof(t_list_item));
	if (!item)
		return (NULL);
	item->type = type;
	item->number = number;
	if (type == PHILOSOPHER)
		item->state = state;
	item->prev = NULL;
	item->next = NULL;
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

	item = first;
	first = first->next;
	free(item);
	while (first != NULL)
	{
		item = first;
		first = first->next;
		free(item);
	}
}
