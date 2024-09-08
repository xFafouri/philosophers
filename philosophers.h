/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:57:00 by hfafouri          #+#    #+#             */
/*   Updated: 2024/09/08 16:32:52 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <fcntl.h>
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include <semaphore.h>
# include <sys/time.h>


typedef struct s_philo
{
    int					nb_philo;
	int					time_death;
	int					time_eat;
	int					time_sleep;
	pthread_t		thread;
	int				id;
	int				meals_eaten;
	size_t			last_meal;
	size_t			time_to_sleep;
	size_t			start_time;
	int             eat_count;
	int             status;
	int             eating;
	int        		time_to_die;
	int				num_of_philos;
	int				num_times_to_eat;
	int				*dead;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	meal_lock;
}   t_philo;


#endif