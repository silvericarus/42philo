/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgonza <albgonza@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:53:22 by albgonza          #+#    #+#             */
/*   Updated: 2023/05/15 20:35:36 by albgonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_arguments(int args, char **argv, t_main *main)
{
	ft_atoi(argv[1], main);
	ft_atoi(argv[2], main);
	ft_atoi(argv[3], main);
	ft_atoi(argv[4], main);
	if (args == 6)
		ft_atoi(argv[5], main);
	if (!main->valid_arguments)
		return (1);
	else
		return (0);
}

void	start_thinking(t_philo *philo)
{
	if (!philo->main_philo->printed_thinking)
		philo_print("%lld %d is thinking\n", philo);
	pthread_mutex_lock(philo->main_philo->printed_thinking_m);
	philo->main_philo->printed_thinking = 1;
	pthread_mutex_unlock(philo->main_philo->printed_thinking_m);
}
