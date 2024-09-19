/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:57:00 by hfafouri          #+#    #+#             */
/*   Updated: 2024/09/18 05:03:18 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <fcntl.h>
# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_shared
{
	int				dead;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	print_lock;
}					t_shared;

typedef struct s_philo
{
	int				id;
	int				ac;
	char			**av;
	int				nb_philo;
	int				num_philos;
	unsigned long	time_death;
	unsigned long	time_eat;
	unsigned long	time_sleep;
	unsigned long	last_meal;
	int				meals_eaten;
	int				all_ate;
	int				num_times_to_eat;
	int				no_time_eat;
	unsigned long	start_time;
	pthread_t		thread;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;
	pthread_mutex_t	meal_lock;
	t_shared		*shared;
}					t_philo;

int					ft_atoi(const char *str);
unsigned long		get_time(void);
void				ft_usleep(unsigned long target_time);
int					lock_forks(t_philo *philo, pthread_mutex_t *first,
						pthread_mutex_t *second);
int					ft_eat(t_philo *philo);
void				*routine(void *arg);
void				sharing_forks(t_philo *philos, pthread_mutex_t *forks,
						int *i);
void				init_var(t_philo *philos, pthread_mutex_t *forks,
						t_shared *shared);
void				ft_create_join_threads(t_philo *philos, int num_philos);
void				ft_start(t_philo *philos, t_shared *shared,
						pthread_mutex_t *forks);
int					thread_status(t_philo *philo, char *message);
int					check_death(t_philo *philos, int i);
void				*supervisor(void *arg);
int					ft_isdigit(int c);
int					check_parsing(char **av, int ac);

#endif