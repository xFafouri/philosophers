/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:00:17 by hfafouri          #+#    #+#             */
/*   Updated: 2024/09/18 03:46:51 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(const char *str)
{
	int	r;
	int	s;

	r = 0;
	s = 1;
	if (str == NULL)
		return (-1);
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			s *= -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		r = r * 10 + (*str - 48);
		str++;
	}
	return (r * s);
}

unsigned long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

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

void	ft_usleep(unsigned long target_time)
{
	unsigned long	start;

	start = get_time();
	while (get_time() - start < target_time)
		usleep(500);
}

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

int	main(int ac, char **av)
{
	t_philo			*philos;
	pthread_mutex_t	*forks;
	t_shared		shared;
	int				num_philos;

	num_philos = ft_atoi(av[1]);
	philos = malloc(sizeof(t_philo) * num_philos);
	forks = malloc(sizeof(pthread_mutex_t) * num_philos);
	philos->num_philos = num_philos;
	memset(&shared, 0, sizeof(t_shared));
	philos->ac = ac;
	philos->av = av;
	ft_start(philos, &shared, forks);
	free(philos);
	free(forks);
	return (0);
}
