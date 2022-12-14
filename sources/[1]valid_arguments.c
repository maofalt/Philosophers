/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   [1]valid_arguments.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:06:37 by motero            #+#    #+#             */
/*   Updated: 2022/12/07 22:23:12 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// A function that checks if a string represents an integer
static int	is_integer(char *s)
{
	int	i;

	i = 0;
	if (s[0] == '-')
		i = 1;
	while (s[i] != 0)
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

// A function that checks if the arguments are valid
int	ft_valid_arguments(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
		return (0);
	if (argv[1][0] == 0 || !is_integer(argv[1]) || ft_atoi(argv[1]) < 1)
		return (0);
	if (argv[2][0] == 0 || !is_integer(argv[2]) || ft_atoi(argv[2]) < 0)
		return (0);
	if (argv[3][0] == 0 || !is_integer(argv[3]) || ft_atoi(argv[3]) < 0)
		return (0);
	if (argv[4][0] == 0 || !is_integer(argv[4]) || ft_atoi(argv[4]) < 0)
		return (0);
	if (argc == 6
		&& (argv[5][0] == 0 || !is_integer(argv[5]) || ft_atoi(argv[5]) < 0))
		return (0);
	return (1);
}
