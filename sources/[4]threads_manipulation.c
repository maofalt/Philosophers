/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   [4]threads_manipulation.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:06:37 by motero            #+#    #+#             */
/*   Updated: 2022/12/07 23:35:23 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_create_threads(t_arguments args, t_list_item *list)
{
	t_list_item *current = list;
	while (current != NULL)
	{
		if (current->type == PHILOSOPHER) {
		current->thread = malloc(sizeof(pthread_t));
		pthread_create(current->thread, NULL, function, current);
	}
	current = current->next;
}
