/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_split_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgonza <albgonza@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 21:01:14 by albgonza          #+#    #+#             */
/*   Updated: 2023/05/18 18:09:08 by albgonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	finish_eating(t_philo *tphilo, long long *die_alarm, int *turns)
{
	long long	eat_alarm;

	eat_alarm = get_time() + tphilo->main_philo->eat_time;
	*die_alarm = get_time() + tphilo->main_philo->die_time;
	if (tphilo->main_philo->playing)
		philo_print("%lld %d is eating\n", tphilo);
	while (tphilo->actual_time <= eat_alarm
		&& tphilo->main_philo->playing)
	{
		tphilo->actual_time = get_time();
		if (tphilo->actual_time >= *die_alarm)
			handle_death(tphilo);
	}
	pthread_mutex_unlock(tphilo->left_fork->f_mutex);
	pthread_mutex_unlock(tphilo->right_fork->f_mutex);
	pthread_mutex_lock(tphilo->main_philo->printed_thinking_m);
	tphilo->main_philo->printed_thinking = 0;
	pthread_mutex_unlock(tphilo->main_philo->printed_thinking_m);
	tphilo->left_fork->taken = 0;
	tphilo->right_fork->taken = 0;
	tphilo->status = SLEEPING;
	(*turns)++;
}

int	loop_check(t_philo *tphilo, int *turns, long long *die_alarm)
{
	int	tmp;

	tmp = (*turns);
	if (tphilo->actual_time >= *die_alarm)
		handle_death(tphilo);
	if (tphilo->main_philo->loop_mode)
	{	
		return (tphilo->main_philo->playing
			&& tmp < tphilo->main_philo->turns);
	}
	else
		return (tphilo->main_philo->playing);
}

void	initialize_fork_mutex(t_main *main, int index)
{
	main->forks_mutexes[index].f_mutex = malloc(sizeof(pthread_mutex_t));
	main->forks_mutexes[index].taken = 0;
	pthread_mutex_init(main->forks_mutexes[index].f_mutex, NULL);
}

void	initialize_philo(t_philo *philo, t_main *main, int index)
{
	if (index % 2 == 0)
	{
		philo->status = SLEEPING;
		philo->left_handed = 1;
	}
	else
	{
		philo->status = THINKING;
		philo->left_handed = 0;
	}
	philo->id = index + 1;
	philo->main_philo = (struct s_main *)main;
	philo->left_fork = &main->forks_mutexes[index];
	if (index != (main->num_of_philos - 1))
		philo->right_fork = &main->forks_mutexes[index + 1];
	else
		philo->right_fork = &main->forks_mutexes[0];
}

int	initialize_main(t_main *main, int args, char **argv)
{
	main->num_of_philos = ft_atoi(argv[1], main);
	main->die_time = ft_atoi(argv[2], main);
	main->eat_time = ft_atoi(argv[3], main);
	main->sleep_time = ft_atoi(argv[4], main);
	if (main->num_of_philos < 1 || main->num_of_philos > 200)
	{
		printf("Error, 1-200 philos!");
		return (1);
	}
	if (args == 5)
	{
		main->loop_mode = 0;
		main->turns = 42;
	}
	else
	{
		main->loop_mode = 1;
		main->turns = ft_atoi(argv[5], main);
	}
	main->table = malloc(main->num_of_philos * sizeof(t_philo));
	main->forks_mutexes = malloc(main->num_of_philos * sizeof(t_fork));
	main->print_turn = malloc(sizeof(pthread_mutex_t));
	main->playing = 1;
	main->initial_time = get_time();
	return (0);
}
