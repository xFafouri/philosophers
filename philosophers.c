/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:00:17 by hfafouri          #+#    #+#             */
/*   Updated: 2024/09/08 16:33:51 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philosophers.h"


int	ft_atoi(const char *str)
{
	int	r;
	int	s;

	r = 0;
	s = 1;
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

unsigned long get_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void thread_status(t_philo *philo, const char *message)
{
    pthread_mutex_lock(&philo->write_lock);
    printf("Philo %d %s\n", philo->id, message);
    pthread_mutex_unlock(&philo->write_lock);
}

void *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    while (1)
    {        
        pthread_mutex_lock(&philo->dead_lock);
        if (*philo->dead)
        {
            pthread_mutex_unlock(&philo->dead_lock);
            break;
        }
        pthread_mutex_unlock(&philo->dead_lock);
        thread_status(philo, "is thinking");
        pthread_mutex_lock(philo->r_fork);
        thread_status(philo, "khda limniya");
        pthread_mutex_lock(philo->l_fork);
        thread_status(philo, "khda lisriya");
        thread_status(philo, "has taken a fork");

        pthread_mutex_lock(&philo->meal_lock);
        // pthread_mutex_lock(&philo->meal_lock);
        philo->eating = 1;
        philo->last_meal = get_time();
        pthread_mutex_unlock(&philo->meal_lock);

        thread_status(philo, "is eating");
        usleep(philo->time_eat); 
        // philo->meals_eaten++;
        pthread_mutex_lock(&philo->meal_lock);
        philo->meals_eaten++;
        philo->eating = 0;
        pthread_mutex_unlock(&philo->meal_lock);

        pthread_mutex_unlock(philo->l_fork);
        pthread_mutex_unlock(philo->r_fork);

        thread_status(philo, "is sleeping");
        usleep(philo->time_sleep);
        if (philo->meals_eaten >= philo->num_times_to_eat)
            break;
    }
    return NULL;
}

void init_var(int ac, char **av, t_philo *philos, pthread_mutex_t *forks)
{
    int i = 0;
    int num_philos = atoi(av[1]);

    while (i < num_philos)
    {
        philos[i].id = i;
        philos[i].nb_philo = num_philos;
        philos[i].time_death =  atoi(av[2]);
        philos[i].time_eat = atoi(av[3]);
        philos[i].time_sleep = atoi(av[4]);
        philos[i].meals_eaten = 0;
        philos[i].last_meal = get_time();
        philos[i].dead = malloc(sizeof(int));
        *philos[i].dead = 0;
        philos[i].r_fork = &forks[i];
        philos[i].l_fork = &forks[(i - 1+ num_philos) % num_philos];
        pthread_mutex_init(&philos[i].write_lock, NULL);
        pthread_mutex_init(&philos[i].dead_lock, NULL);
        pthread_mutex_init(&philos[i].meal_lock, NULL);
        i++;
    }
}


void ft_create_join_threads(t_philo *philos, char **av)
{
    int i = 0;
    while (i < ft_atoi(av[1]))
    {
        pthread_create(&philos[i].thread, NULL, routine, (void *)&philos[i]);
        i++;
    }
    i = 0;
    while (i < ft_atoi(av[1]))
    {
        pthread_join(philos[i].thread, NULL);
        i++;
    }
}

void *supervisor(void *arg)
{
    t_philo *philos = (t_philo *)arg;
    int i = 0;
    while (1)
    {
        i = 0;
        while (i < philos->nb_philo)
        {
            // mutex lock for eating
            if (philos[i].eating == 0 && (get_time() - philos[i].last_meal >= philos[i].time_death))
            {
                pthread_mutex_lock(&philos[i].dead_lock);
                *philos[i].dead = 1;
                pthread_mutex_unlock(&philos[i].dead_lock);
                thread_status(&philos[i], "is dead");
                return NULL;
            }
            i++;
        }
        usleep(750);
    }
    return NULL;
}

void *monitor(void *arg)
{
    t_philo *philos = (t_philo *)arg;
    int all_done = 0;
    int i = 0;
    while (!all_done)
    {
        all_done = 1;
        i = 0;
        while (i < philos->nb_philo)
        {
            if (philos[i].meals_eaten < philos[i].num_times_to_eat)
            {
                all_done = 0;
                break;
            }
            i++;
        }
    }
    return NULL;
}

int main(int ac, char **av)
{
    t_philo *philos = malloc(sizeof(t_philo) * (ft_atoi(av[1]) + 1));
    pthread_mutex_t *forks = malloc(sizeof(pthread_mutex_t) * atoi(av[1]));

    init_var(ac, av, philos, forks); 

    pthread_t monitor_thread;
    pthread_t supervisor_thread;
    pthread_create(&supervisor_thread, NULL, supervisor, (void *)philos);
    pthread_create(&monitor_thread, NULL, monitor, (void *)philos);

    ft_create_join_threads(philos, av);
    pthread_join(supervisor_thread, NULL);
    pthread_join(monitor_thread, NULL);

    int i = 0;

    while (i < atoi(av[1]))
    {
        pthread_mutex_init(&forks[i], NULL);
        i++;
    }
    i = 0;
    while (i < ft_atoi(av[1]))
    {
        pthread_mutex_destroy(&philos[i].write_lock);
        pthread_mutex_destroy(&philos[i].dead_lock);
        pthread_mutex_destroy(&philos[i].meal_lock);
        i++;
    }
    return 0;
}
