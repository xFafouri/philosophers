/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:00:17 by hfafouri          #+#    #+#             */
/*   Updated: 2024/09/17 23:45:18 by hfafouri         ###   ########.fr       */
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
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

int	thread_status(t_philo *philo, char *message)
{
	int	should_exit;

	should_exit = 0;
	pthread_mutex_lock(&philo->shared->dead_lock);
	if (philo->shared->dead)
		should_exit = 1;
	else
	{
		pthread_mutex_lock(&philo->shared->print_lock);
		printf("%lu %d %s\n", get_time() - philo->start_time, philo->id + 1,
			message);
		pthread_mutex_unlock(&philo->shared->print_lock);
	}
	pthread_mutex_unlock(&philo->shared->dead_lock);
	return (should_exit);
}

void	ft_usleep(unsigned long target_time)
{
	unsigned long	start;

	start = get_time();
	while (get_time() - start < target_time)
		usleep(500);
}

int	check_forks(t_philo *philo, pthread_mutex_t *first, pthread_mutex_t *second)
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

void	*routine(void *arg)
{
    t_philo	*philo;

    philo = (t_philo *)arg;
    if (philo->id % 2 != 0) 
        usleep(philo->time_eat / 2);
    while (1)
    {
		// if (philo->id % 2 == 1)
		// 	usleep(500);
        if (thread_status(philo, "is thinking"))
            break;
		
        if (philo->id % 2 == 0)
        {
            if (check_forks(philo, philo->first_fork, philo->second_fork))
                break;
        }
        else 
        {
            if (check_forks(philo, philo->second_fork, philo->first_fork))
                break;
        }
        pthread_mutex_lock(&philo->meal_lock);
        philo->last_meal = get_time();
        philo->meals_eaten++;
        pthread_mutex_unlock(&philo->meal_lock);
        if (thread_status(philo, "is eating"))
        {
            pthread_mutex_unlock(philo->first_fork);
            pthread_mutex_unlock(philo->second_fork);
            break;
        }
        ft_usleep(philo->time_eat);
        if (philo->id % 2 == 0)
        {
            pthread_mutex_unlock(philo->second_fork);
            pthread_mutex_unlock(philo->first_fork);
        }
        else
        {
            pthread_mutex_unlock(philo->first_fork);
            pthread_mutex_unlock(philo->second_fork);
        }

        if (thread_status(philo, "is sleeping"))
            break;
        ft_usleep(philo->time_sleep);
        pthread_mutex_lock(&philo->shared->dead_lock);
        if (philo->shared->dead || (philo->no_time_eat == 0 && philo->meals_eaten >= philo->num_times_to_eat))
        {
            pthread_mutex_unlock(&philo->shared->dead_lock);
            break;
        }
        pthread_mutex_unlock(&philo->shared->dead_lock);
    }
    return (NULL);
}


void	init_var(int ac, char **av, t_philo *philos, pthread_mutex_t *forks,
		t_shared *shared)
{
	int				num_philos;
	unsigned long	start_time;
	int				i;

	num_philos = ft_atoi(av[1]);
	start_time = get_time();
	i = 0;
	while (i < num_philos)
	{
		philos[i].id = i;
		philos[i].nb_philo = ft_atoi(av[1]);
		philos[i].time_death = ft_atoi(av[2]);
		philos[i].time_eat = ft_atoi(av[3]);
		philos[i].time_sleep = ft_atoi(av[4]);
		philos[i].meals_eaten = 0;
		philos[i].last_meal = start_time;
		if (ac == 6)
			philos[i].num_times_to_eat = ft_atoi(av[5]);
		else
			philos[i].num_times_to_eat = -1;
		philos[i].no_time_eat = (ac == 5);
		philos[i].start_time = start_time;
		philos[i].shared = shared;
		if (i == philos[i].nb_philo)
		{
			philos[i].first_fork = &forks[(i + 1) % num_philos];
			philos[i].second_fork = &forks[i];
		}
		else if (i % 2 == 0)
		{
			philos[i].first_fork = &forks[i];
			philos[i].second_fork = &forks[(i + 1) % num_philos];
		}
		else
		{
			philos[i].first_fork = &forks[(i + 1) % num_philos];
			philos[i].second_fork = &forks[i];
		}
		pthread_mutex_init(&philos[i].meal_lock, NULL);
		// printf("id = %d | time_death = %lu | time_eat = %lu | time_sleep = %lu |\n\n", philos[i].id, philos[i].time_death, philos[i].time_eat, philos[i].time_sleep);
		i++;
	}
}

void	*supervisor(void *arg)
{
	t_philo			*philos;
	int				i;
	int				all_ate;
	unsigned long	current_time;

	philos = (t_philo *)arg;
	while (1)
	{
		all_ate = 1;
		i = 0;
		while (i < philos[0].nb_philo)
		{
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
				return (NULL);
			}
			if (philos[i].no_time_eat == 0
				&& philos[i].meals_eaten < philos[i].num_times_to_eat)
				all_ate = 0;
			pthread_mutex_unlock(&philos[i].meal_lock);
			i++;
		}
		if (all_ate && philos[0].no_time_eat == 0)
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

// 4 310 200 100 one philo should die

//./philosophers 4 410 200 200 7 > out
// cat out | grep "2 is eating" | wc -l
int	main(int ac, char **av)
{
	t_philo			*philos;
	pthread_mutex_t	*forks;
	t_shared		shared;
	int				num_philos;
	int				i;

	num_philos = ft_atoi(av[1]);
	philos = malloc(sizeof(t_philo) * num_philos);
	forks = malloc(sizeof(pthread_mutex_t) * num_philos);
	memset(&shared, 0, sizeof(t_shared));
	pthread_mutex_init(&shared.dead_lock, NULL);
	pthread_mutex_init(&shared.print_lock, NULL);
	i = 0;
	while (i < num_philos)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	init_var(ac, av, philos, forks, &shared);
	ft_create_join_threads(philos, num_philos);
	i = 0;
	while (i < num_philos)
	{
		pthread_mutex_destroy(&philos[i].meal_lock);
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	pthread_mutex_destroy(&shared.dead_lock);
	pthread_mutex_destroy(&shared.print_lock);
	free(philos);
	free(forks);
	return (0);
}
