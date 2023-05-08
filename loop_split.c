/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgonza <albgonza@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 19:34:38 by albgonza          #+#    #+#             */
/*   Updated: 2023/05/08 20:27:29 by albgonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_free(t_main *main)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(main->print_turn);
	pthread_mutex_destroy(main->died_print_m);
	while (i < main->num_of_philos)
	{
		pthread_mutex_destroy(main->forks_mutexes[i].f_mutex);
		i++;
	}
}
