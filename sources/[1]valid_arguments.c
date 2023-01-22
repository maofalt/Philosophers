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

// A function that checks if the arguments are valid
int	ft_valid_arguments(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
		return (0);
	if (argv[1][0] == 0 || !valid_integer(argv[1]) || ft_atoi(argv[1]) < 1)
		return (0);
	if (argv[2][0] == 0 || !valid_integer(argv[2]) || ft_atoi(argv[2]) < 0)
		return (0);
	if (argv[3][0] == 0 || !valid_integer(argv[3]) || ft_atoi(argv[3]) < 0)
		return (0);
	if (argv[4][0] == 0 || !valid_integer(argv[4]) || ft_atoi(argv[4]) < 0)
		return (0);
	if (argc == 6
		&& (argv[5][0] == 0 || !valid_integer(argv[5]) || ft_atoi(argv[5]) < 0))
		return (0);
	return (1);
}


/*Know if the string is a valid integer : No bigger 
**than INT_MAX and no smaller than INT_MIN*/
int	valid_integer(char *str)
{
	int	i;
	int	neg;
	int	res;

	i = 0;
	neg = 1;
	res = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
		neg = -1;
	if (str[i] == '-')
		i++;
	while (str[i] == '0')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (res > INT_MAX / 10 || (res == INT_MAX / 10 && str[i] - '0' > 7))
			return (0);
		if (neg == -1 && (res < INT_MIN / 10
				|| (res == INT_MIN / 10 && str[i] - '0' > 8)))
			return (0);
		res = res * 10 + str[i++] - '0';
	}
	return (str[i] == '\0');
}
