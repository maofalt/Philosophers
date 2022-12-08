/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   [4]threads_manipulation.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:06:37 by motero            #+#    #+#             */
/*   Updated: 2022/12/08 18:49:28 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_create_threads(t_list_item *list)
{
	t_list_item	*current;

	current = list;
	while (current)
	{
		if (current->type == PHILOSOPHER)
		{
			pthread_create(current->thread, NULL, philosopher_thread, current);
		}
		current = current->next;
		if (current == list)
			break ;
	}
}
