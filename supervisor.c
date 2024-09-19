/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervisor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 03:54:22 by hfafouri          #+#    #+#             */
/*   Updated: 2024/09/18 03:55:09 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	thread_status(t_philo *philo, char *message)
{
	pthread_mutex_lock(&philo->shared->dead_lock);
	if (philo->shared->dead)
	{
		pthread_mutex_unlock(&philo->shared->dead_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->shared->dead_lock);
	pthread_mutex_lock(&philo->shared->print_lock);
	printf("%lu %d %s\n", get_time() - philo->start_time, philo->id + 1,
		message);
	pthread_mutex_unlock(&philo->shared->print_lock);
	return (0);
}

int	check_death(t_philo *philos, int i)
{
	unsigned long	current_time;

	current_time = get_time();
	pthread_mutex_lock(&philos[i].meal_lock);
	if (current_time - philos[i].last_meal > philos[i].time_death)
	{
		pthread_mutex_lock(&philos[i].shared->dead_lock);
		philos[i].shared->dead = 1;
		pthread_mutex_unlock(&philos[i].shared->dead_lock);
		pthread_mutex_lock(&philos[i].shared->print_lock);
		printf("%lu %d died\n", current_time - philos[i].start_time,
			philos[i].id + 1);
		pthread_mutex_unlock(&philos[i].shared->print_lock);
		pthread_mutex_unlock(&philos[i].meal_lock);
		return (1);
	}
	if (philos[i].no_time_eat == 0
		&& philos[i].meals_eaten < philos[i].num_times_to_eat)
		philos->all_ate = 0;
	pthread_mutex_unlock(&philos[i].meal_lock);
	return (0);
}

void	*supervisor(void *arg)
{
	t_philo	*philos;
	int		i;

	philos = (t_philo *)arg;
	while (1)
	{
		philos->all_ate = 1;
		i = 0;
		while (i < philos[0].nb_philo)
		{
			if (check_death(philos, i))
				return (NULL);
			i++;
		}
		if (philos->all_ate && philos[0].no_time_eat == 0)
		{
			pthread_mutex_lock(&philos[0].shared->dead_lock);
			philos[0].shared->dead = 1;
			pthread_mutex_unlock(&philos[0].shared->dead_lock);
			return (NULL);
		}
	}
}
