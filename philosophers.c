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

int j = 0;


// sem_t semaphore;

void* test()
{    
    // sem_wait(&semaphore);
    int lock = 0;
    int i = 0;
        while(i < 1000000)
        {
            pthread_mutex_lock(&mutex);
            j++; 
            i++;
            pthread_mutex_unlock(&mutex);
        }
        // sem_post(&semaphore);
    return(NULL);
}

void init_variable(int ac, char **av, t_philo *data)
{

    if (ac <= 1)
    {
        write(2,"ARG ERROR\n", 11);
        exit(1);
    }
    data->nb_philo = atoi(av[1]);
    data->time_death = atoi(av[2]);
    data->time_eat = atoi(av[3]);
    data->time_sleep = atoi(av[4]);
}


int main(int ac, char **av)
{
    t_philo data;

    int i = 0;
    init_variable(ac, av, &data);
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    // sem_init(&semaphore, 0,2);
    pthread_t t1[data.nb_philo];

    while(i < data.nb_philo)
    {
        pthread_create(&t1[i], NULL,&test,NULL);
        printf("thread bda\n");
        pthread_join(t1[i], NULL);
        printf("thread sala\n");
        i++;
    }
    //pthread_create(&t2, NULL,&test,NULL);
    // pthread_join(t2, NULL);
    pthread_mutex_destroy(&mutex);
    // sem_destroy(&semaphore);
    printf("i = %d\n", j);

}