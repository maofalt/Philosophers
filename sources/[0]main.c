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
	t_arguments	*args;
	t_list_item	*list;

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
	free_list(list);
	free(args);
}
