/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_start.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 03:53:01 by hfafouri          #+#    #+#             */
/*   Updated: 2024/09/18 03:57:09 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	sharing_forks(t_philo *philos, pthread_mutex_t *forks, int *i)
{
	if (*i + 1 == philos[*i].nb_philo)
	{
		philos[*i].first_fork = &forks[((*i) + 1) % philos->num_philos];
		philos[*i].second_fork = &forks[*i];
	}
	else
	{
		philos[*i].first_fork = &forks[*i];
		philos[*i].second_fork = &forks[((*i) + 1) % philos->num_philos];
	}
}

void	init_var(t_philo *philos, pthread_mutex_t *forks,
		t_shared *shared)
{
	unsigned long	start_time;
	int				i;

	start_time = get_time();
	i = 0;
	while (i < philos->num_philos)
	{
		philos[i].id = i;
		philos[i].nb_philo = ft_atoi(philos->av[1]);
		philos[i].time_death = ft_atoi(philos->av[2]);
		philos[i].time_eat = ft_atoi(philos->av[3]);
		philos[i].time_sleep = ft_atoi(philos->av[4]);
		philos[i].meals_eaten = 0;
		philos[i].last_meal = start_time;
		if (philos->ac == 6)
			philos[i].num_times_to_eat = ft_atoi(philos->av[5]);
		else
			philos[i].num_times_to_eat = -1;
		philos[i].no_time_eat = (philos->ac == 5);
		philos[i].start_time = start_time;
		philos[i].shared = shared;
		sharing_forks(philos, forks, &i);
		pthread_mutex_init(&philos[i].meal_lock, NULL);
		i++;
	}
}

void	ft_create_join_threads(t_philo *philos, int num_philos)
{
	int			i;
	pthread_t	supervisor_thread;

	i = 0;
	while (i < num_philos)
	{
		pthread_create(&philos[i].thread, NULL, routine, (void *)&philos[i]);
		i++;
	}
	pthread_create(&supervisor_thread, NULL, supervisor, (void *)philos);
	pthread_join(supervisor_thread, NULL);
	i = 0;
	while (i < num_philos)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
}

void	ft_start(t_philo *philos, t_shared *shared, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	pthread_mutex_init(&shared->dead_lock, NULL);
	pthread_mutex_init(&shared->print_lock, NULL);
	i = 0;
	while (i < philos->num_philos)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	init_var(philos, forks, shared);
	ft_create_join_threads(philos, philos->num_philos);
	i = 0;
	while (i < philos->num_philos)
	{
		pthread_mutex_destroy(&philos[i].meal_lock);
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	pthread_mutex_destroy(&shared->dead_lock);
	pthread_mutex_destroy(&shared->print_lock);
}
