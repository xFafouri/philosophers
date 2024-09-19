/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 03:51:44 by hfafouri          #+#    #+#             */
/*   Updated: 2024/09/18 03:56:40 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	lock_forks(t_philo *philo, pthread_mutex_t *first, pthread_mutex_t *second)
{
	pthread_mutex_lock(first);
	if (thread_status(philo, "has taken a fork"))
	{
		pthread_mutex_unlock(first);
		return (1);
	}
	pthread_mutex_lock(second);
	if (thread_status(philo, "has taken a fork"))
	{
		pthread_mutex_unlock(first);
		pthread_mutex_unlock(second);
		return (1);
	}
	return (0);
}

int	ft_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_lock);
	if (thread_status(philo, "is eating"))
	{
		pthread_mutex_unlock(philo->first_fork);
		pthread_mutex_unlock(philo->second_fork);
		return (1);
	}
	ft_usleep(philo->time_eat);
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 != 0)
		ft_usleep(philo->time_eat / 2);
	while (1)
	{
		if (lock_forks(philo, philo->first_fork, philo->second_fork))
			return (NULL);
		if (ft_eat(philo))
			return (NULL);
		pthread_mutex_unlock(philo->first_fork);
		pthread_mutex_unlock(philo->second_fork);
		if (thread_status(philo, "is sleeping"))
			return (NULL);
		ft_usleep(philo->time_sleep);
		if (thread_status(philo, "is thinking"))
			return (NULL);
		ft_usleep((philo->time_death - (get_time() - philo->last_meal)) / 2);
	}
	return (NULL);
}
