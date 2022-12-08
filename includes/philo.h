/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 11:36:43 by motero            #+#    #+#             */
/*   Updated: 2022/12/08 22:19:29 by motero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include "../gnl/get_next_line.h"
# include "libft.h"

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
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
	SLEEPING,
	THINKING,
	HUNGRY,
	FORKING,
	EATING,
	FULL,
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
	t_arguments				args;
	t_item_type				type;
	t_state					state;
	int						number;
	struct s_list_item		*next;
	struct s_list_item		*prev;
	pthread_mutex_t			mutex;
	pthread_t				*thread;
}	t_list_item;

// Struct to store information about a philosopher
typedef struct s_philosopher {
	int						nb;
	int						eat_count;
	int						think_time;
	int						eat_time;
	int						sleep_time;
	int						time_to_die;
	int						time_to_eat;
	int						time_to_sleep;
	int						num_times_to_eat;
	pthread_mutex_t			*left_fork;
	pthread_mutex_t			*right_fork;
}	t_philosopher;

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
t_list_item		*create_item(t_item_type type, t_state state,
					t_arguments a, int nb);
void			link_items(t_list_item *prev, t_list_item *next);
void			ft_print_list(t_list_item *first);
void			free_list(t_list_item *first);

/*############################################################################*/
/*                           THREADS_MANIPULATION							  */
/*############################################################################*/

void			ft_create_threads(t_list_item *list);

/*############################################################################*/
/*                           PHILOSOPHERS BEHAVIOR							  */
/*############################################################################*/

void			*philosopher_thread(void *arg);
void			ft_display_status(struct timeval start, t_list_item *philosopher);
int				ft_try_eat(struct timeval start, t_list_item *philosopher);
void			ft_put_down_forks(t_list_item *philosopher);
void			ft_philo_starved(struct timeval start, t_list_item *philosopher);

#endif