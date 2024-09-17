/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:57:00 by hfafouri          #+#    #+#             */
/*   Updated: 2024/09/15 05:42:50 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <fcntl.h>
# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>
# include <string.h>


typedef struct s_shared
{
    int dead;
    pthread_mutex_t dead_lock;
    pthread_mutex_t print_lock;
} t_shared;

typedef struct s_philo
{
    int id;
    int nb_philo;
    unsigned long time_death;
    unsigned long time_eat;
    unsigned long time_sleep;
    unsigned long last_meal;
    int meals_eaten;
    int num_times_to_eat;
    int no_time_eat;
    unsigned long start_time;
    pthread_t thread;
    pthread_mutex_t *first_fork;
    pthread_mutex_t *second_fork;
    pthread_mutex_t meal_lock;
    t_shared *shared;
} t_philo;


#endif