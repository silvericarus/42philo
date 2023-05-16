/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgonza <albgonza@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:53:22 by albgonza          #+#    #+#             */
/*   Updated: 2023/05/16 18:29:13 by albgonza         ###   ########.fr       */
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

void	start_thinking(t_philo *philo, long long *die_alarm)
{
	long long	think_alarm;

	think_alarm = get_time() + 100;
	if (!philo->main_philo->printed_thinking)
		philo_print("%lld %d is thinking\n", philo);
	pthread_mutex_lock(philo->main_philo->printed_thinking_m);
	philo->main_philo->printed_thinking = 1;
	pthread_mutex_unlock(philo->main_philo->printed_thinking_m);
	while (philo->actual_time <= think_alarm
		&& philo->main_philo->playing)
	{
		philo->actual_time = get_time();
		if (philo->actual_time >= *die_alarm)
			handle_death(philo);
		if (should_take_forks(philo))
			break ;
	}
}

void	print_table_data(t_main *main)
{
	int	i;

	i = 0;
	while (i < main->num_of_philos)
	{
		printf("philo id:%d has left fork %d and right fork %d\n",
			main->table[i].id, main->table[i].left_fork->order,
			main->table[i].right_fork->order);
		i++;
	}
}
