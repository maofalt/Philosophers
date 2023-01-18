/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motero <motero@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 11:36:43 by motero            #+#    #+#             */
/*   Updated: 2023/01/16 17:48:49 by motero           ###   ########.fr       */
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

//A structure to store the time
typedef struct s_timestamps {
	struct timeval	start;
	struct timeval	current;
	struct timeval	start_last_meal;
	int				delta_time;
	int				delta_last_meal;
}	t_timestamps;

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
	DEAD,
	END,
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

typedef struct s_thread_info {
	t_timestamps	*timestamps;
	int				times_eaten;
	int				nbr_philo_full;
	int				someone_died;
	pthread_mutex_t	*display_mutex;
	pthread_mutex_t	*death_mutex;
}					t_thread_info;

typedef struct s_list_item {
	t_arguments				args;
	t_item_type				type;
	t_state					state;
	int						number;
	struct s_list_item		*next;
	struct s_list_item		*prev;
	pthread_mutex_t			mutex;
	pthread_t				*thread;
	t_thread_info			*info;
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
void			link_items(t_list_item *prev, t_list_item *next);
void			ft_print_list(t_list_item *first);
void			free_list(t_list_item *first);

/*############################################################################*/
/*                           ITEM  CONSTRUCTOR							      */
/*############################################################################*/
t_list_item		*create_item(t_item_type type, t_state state,
					t_arguments a, int nb);
int				ft_init_first_philo_fork(t_list_item **first,
					t_list_item **fork, t_arguments *args);
int				ft_create_philo(t_list_item **philosopher, t_arguments *args,
					int i, t_list_item **first);
int				ft_create_fork(t_list_item **fork, t_arguments *args, int i,
					t_list_item **first);
int				ft_philo_allocation(t_list_item *item, t_item_type type,
					int nb);

/*############################################################################*/
/*                    INFO STRUCT MANIPULATION							      */
/*############################################################################*/

t_thread_info	*ft_create_info(void);

/*############################################################################*/
/*                           THREADS_MANIPULATION							  */
/*############################################################################*/

void			ft_create_threads(t_list_item *list);
void			ft_init_shared_time(t_list_item *current, struct timeval start);
void			ft_init_shared_info(t_list_item *current, int *nbr_philo_full,
					int *someone_died);

/*############################################################################*/
/*                           PHILOSOPHERS BEHAVIOR							  */
/*############################################################################*/

void			*philo_thread(void *arg);
void			ft_display_status(struct timeval start, t_list_item *philo);
int				ft_try_eat(struct timeval start, t_list_item *philo);
void			ft_put_down_forks(t_list_item *philo);
int				ft_philo_starved(struct timeval start, t_list_item *philo);
int				ft_stop_signal(t_list_item *philo);

/*############################################################################*/
/*                           MONITOR	BEHAVIOR							  */
/*############################################################################*/
void			*monitor_philosophers(void *arg);
void			stop_philosophers(t_list_item *phlo);

#endif