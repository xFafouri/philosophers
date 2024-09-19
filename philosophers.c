/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:00:17 by hfafouri          #+#    #+#             */
/*   Updated: 2024/09/19 06:57:06 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// void check_param(int ac, char av)
// {
// 	if (ft_atoi(av[0]) == 0)
// 		return (0);
// 	if (	
// }

int	main(int ac, char **av)
{
	t_philo			*philos;
	pthread_mutex_t	*forks;
	t_shared		shared;
	int				num_philos;

	if (check_parsing(av, ac) == 0)
	{
		write(2, "Error Args\n", 12);
		exit(0);
	}
	if (ft_atoi(av[1]) == 0 || ft_atoi(av[5]) == 0)
		return (0);
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
