/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   [4]threads_manipulation.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:06:37 by motero            #+#    #+#             */
/*   Updated: 2022/12/08 23:02:09 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_create_threads(t_list_item *list)
{
	t_list_item		*current;
	t_thread_info	*thread_info;
	pthread_mutex_t	display_mutex;

	pthread_mutex_init(&display_mutex, NULL);
	current = list;
	while (current)
	{
		thread_info = malloc(sizeof(t_thread_info));
		if (!thread_info)
			return ;
		thread_info->item = current;
		thread_info->display_mutex = display_mutex;
		if (current->type == PHILOSOPHER)
		{
			pthread_create(current->thread, NULL, philosopher_thread, thread_info);
		}
		current = current->next;
		if (current == list)
			break ;
	}
}
