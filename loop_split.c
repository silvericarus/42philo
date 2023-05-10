/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgonza <albgonza@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 19:34:38 by albgonza          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/05/10 21:59:38 by albgonza         ###   ########.fr       */
=======
/*   Updated: 2023/05/10 21:42:51 by albgonza         ###   ########.fr       */
>>>>>>> e66d72852ed77f38f1c85620ac176e2b80334298
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_free(t_main *main)
{
	int	i;

	i = 0;
	free(main->print_turn);
	free(main->died_print_m);
	pthread_mutex_destroy(main->print_turn);
	pthread_mutex_destroy(main->died_print_m);
	while (i < main->num_of_philos)
	{
		pthread_mutex_destroy(main->forks_mutexes[i].f_mutex);
		free(main->forks_mutexes[i].f_mutex);
		i++;
	}
	free(main->forks_mutexes);
}
