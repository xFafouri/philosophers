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
        thread_status(philo, "is thinking");
        pthread_mutex_lock(philo->r_fork);
        thread_status(philo, "has taken the right fork");
        pthread_mutex_lock(philo->l_fork);
        thread_status(philo, "has taken the left fork");

        thread_status(philo, "is eating");
        philo->last_meal = get_time();
        usleep(philo->time_eat * 1000);
        pthread_mutex_lock(&philo->meal_lock);
        philo->meals_eaten++;
        pthread_mutex_unlock(&philo->meal_lock);

        pthread_mutex_unlock(philo->l_fork);
        pthread_mutex_unlock(philo->r_fork);
        thread_status(philo, "has dropped both forks");
        thread_status(philo, "is sleeping");
        usleep(philo->time_sleep * 1000);
    }
    return NULL;
}

void init_var(int ac, char **av, t_philo **philos)
{
    int i = 0;
    if (ac < 5) 
    {
        write(2, "ARG ERROR\n", 11);
        exit(1);
    }
     while (i < ft_atoi(av[1])) 
    {
        philos[i]->id = i;
        philos[i]->nb_philo = ft_atoi(av[1]);
        philos[i]->time_death = ft_atoi(av[2]);
        philos[i]->time_eat = ft_atoi(av[3]);
        philos[i]->time_sleep = ft_atoi(av[4]);
        philos[i]->meals_eaten = 0;
        philos[i]->last_meal = get_time();
        philos[i]->dead = malloc(sizeof(int));
        *philos[i]->dead = 0;
        pthread_mutex_init(&philos[i]->write_lock, NULL);
        pthread_mutex_init(&philos[i]->dead_lock, NULL);
        pthread_mutex_init(&philos[i]->meal_lock, NULL);
        i++;
    }
}

void    ft_create_join_threads(t_philo *philos, char **av)
{
    int i = 0;
    while (i < ft_atoi(av[1]))
    {
        pthread_create(&philos[i].thread, NULL, routine, (void *)&philos[i]);
        i++;
    }
    i = 0;
    while(i < ft_atoi(av[1])) 
    {
        pthread_join(philos[i].thread, NULL);
        i++;
    }
}

void *supervisor(void *arg)
{
    t_philo *philos = (t_philo *)arg;
    while (1)
    {
        while(i < philos->nb_philo)
        {
            if (philos[i].eating == 0 && (get_time() - philos[i].last_meal >= philos[i].time_to_die)) 
            {
                pthread_mutex_lock(&philos[i].dead_lock);
                *philos[i].dead = 1;
                pthread_mutex_unlock(&philos[i].dead_lock);
                status(&philos[i], "is dead");
            }
        }
        usleep(500);
    }
    return NULL;
}

void *monitor(void *arg)
{
    t_philo *philos = (t_philo *)arg;
    int all_done = 0;
    while (!all_done)
    {
        all_done = 1;
        while(i < philos->nb_philo) 
        {
            if (philos[i].meals_eaten < philos[i].num_times_to_eat) 
            {
                all_done = 0;
                break;
            }
            i++;
        }
        if (all_done) 
        {
            thread_status(philos, "all philosophers have eaten enough");
        }
        sleep(1);
    }
    return NULL;
}

int main(int ac, char **av) 
{
    t_philo *philos = malloc(sizeof(t_philo) * (ft_atoi(av[1]) + 1));

    init_var(ac, av, philos);
    pthread_t monitor_thread;
    pthread_t   supervisor_thread;
    pthread_create(&supervisor_thread, NULL, supervisor, (void *)philos);
    pthread_create(&monitor_thread, NULL, monitor, (void *)philos);

    ft_create_join_threads(philos, av);
    pthread_join(supervisor_thread, NULL);
    pthread_join(monitor_thread, NULL);

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
