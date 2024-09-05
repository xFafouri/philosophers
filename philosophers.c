/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:00:17 by hfafouri          #+#    #+#             */
/*   Updated: 2024/09/05 15:01:39 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philosophers.h"

int j = 0;

pthread_mutex_t mutex; 

void* test()
{
    int lock = 0;
    int i = 0;
        while(i < 1000000)
        {
            pthread_mutex_lock(&mutex);
            j++; 
            i++;
            pthread_mutex_unlock(&mutex);
        }
    return(NULL);
}


int main(int ac, char **av)
{
    int nb = 0;
    int i = 0;
    if (ac <= 1)
        return(write(2,"ARG ERROR\n", 11));
    nb = atoi(av[1]);
    pthread_mutex_init(&mutex, NULL);
    pthread_t t1[nb];

    while(i < nb)
    {
        pthread_create(&t1[i], NULL,&test,NULL);
        printf("thread bda\n");
        pthread_join(t1[i], NULL);
        printf("thread sala\n");
        i++;
    }
    //pthread_create(&t2, NULL,&test,NULL);
    //pthread_join(t2, NULL);
    pthread_mutex_destroy(&mutex);
    printf("i = %d\n", j);

}