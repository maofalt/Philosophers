/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   [0]main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:06:37 by motero            #+#    #+#             */
/*   Updated: 2022/12/08 22:04:47 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_arguments		*args;
	t_list_item		*list;
	t_thread_info	*info;

	if (!ft_valid_arguments(argc, argv))
	{
		ft_printf("Invalid arguments\n");
		return (0);
	}
	args = ft_parse_arguments(argc, argv);
	if (!args)
		return (0);
	list = ft_create_list(args);
	if (!list)
		return (free(args), 0);
	ft_create_threads(list);
	info = list->info;
	ft_wait_threads(list);
	free_list(list);
	free_info(info);
	free(args);
}

void	ft_wait_threads(t_list_item *list)
{
	t_list_item	*item;
	int			i;
	int			max;

	item = list;
	list = list->next;
	i = 0;
	max = item->args.number_of_philosophers * 2;
	while (i++ < max)
	{
		if (item->type == PHILOSOPHER)
		{
			pthread_join(*item->thread, NULL);

		}
		if (item->next)
			item = item->next;
	}
}