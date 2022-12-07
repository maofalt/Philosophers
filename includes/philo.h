/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 11:36:43 by motero            #+#    #+#             */
/*   Updated: 2022/12/07 23:10:17 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include "../gnl/get_next_line.h"
# include "libft.h"

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>

/*############################################################################*/
/*                              STRUCTURES                                    */
/*############################################################################*/

// A structure that contains the parsed arguments
typedef struct s_arguments {
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_times_each_philosopher_must_eat;
}	t_arguments;

typedef enum e_state {
	THINKING,
	EATING,
	SLEEPING,
	DEAD
}	t_state;

// The type of a list item
typedef enum e_item_type {
	PHILOSOPHER,
	FORK,
}	t_item_type;

// A structure that represents an item in the linked list
// 	- The type of the item (philosopher or fork)
// 	- The philosopher or fork id
// 	- A pointer to the next item in the list
// 	- A pointer to the previous item in the list
typedef struct s_list_item {
	t_item_type				type;
	t_state					state;
	int						number;
	struct s_list_item		*next;
	struct s_list_item		*prev;
	pthread_mutex_t			mutex;
	pthread_t				*thread;
}	t_list_item;

/*############################################################################*/
/*                           VALID_ARGUMENTS							      */
/*############################################################################*/

int				ft_valid_arguments(int argc, char **argv);

/*############################################################################*/
/*                           PARSE_ARGUMENTS							      */
/*############################################################################*/

t_arguments		*ft_parse_arguments(int argc, char **argv);

/*############################################################################*/
/*                           LIST_MANIPULATION							      */
/*############################################################################*/

t_list_item		*ft_create_list(t_arguments *args);
t_list_item		*create_item(t_item_type type, t_state state, int number);
void			link_items(t_list_item *prev, t_list_item *next);
void			ft_print_list(t_list_item *first);
void			free_list(t_list_item *first);

/*############################################################################*/
/*                           THREADS_MANIPULATION							  */
/*############################################################################*/

void			ft_create_threads(t_arguments args, t_list_item *list);

/*############################################################################*/
/*                           PHILOSOPHERS BEHAVIOR							  */
/*############################################################################*/

#endif