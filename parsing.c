/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfafouri <hfafouri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 05:02:33 by hfafouri          #+#    #+#             */
/*   Updated: 2024/09/18 05:03:00 by hfafouri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	valid_arg1(char **av, int ac)
{
	int	i;
	int	j;

	i = 1;
	while (i < ac)
	{
		j = 0;
		if (strlen(av[i]) == 0)
			return(0);
		while (av[i][j])
		{
			if (av[i][0] == '+')
				j++;
			if (!ft_isdigit(av[i][j]))
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int	valid_arg2(char **av, int ac)
{
	int	i;
	int	j;

	i = 1;
	while (i < ac)
	{
		j = 0;
		while (av[i][j] && (av[i][j] == ' ' || av[i][j] == '\t'))
			j++;
		while (av[i][j])
		{
			if (av[i][j] == ' ' || av[i][j] == '\t')
			{
				while (av[i][j] == ' ' || av[i][j] == '\t')
					j++;
				if (av[i][j] != '\0')
					return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

int	check_parsing(char **av, int ac)
{
	if (ac != 5 && ac != 6)
		return (0);
	if (!valid_arg1(av, ac))
		return (0);
	if (!valid_arg2(av, ac))
		return (0);
	return (1);
}
