/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_split_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgonza <albgonza@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 21:01:14 by albgonza          #+#    #+#             */
/*   Updated: 2023/05/11 22:06:36 by albgonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	finish_eating(t_philo *tphilo, long long *die_alarm, int *turns)
{
	*die_alarm = get_time() + tphilo->main_philo->die_time;
	philo_print("%lld %d is eating\n", tphilo);
	pthread_mutex_unlock(tphilo->left_fork.f_mutex);
	pthread_mutex_unlock(tphilo->right_fork.f_mutex);
	tphilo->left_fork.taken = 0;
	tphilo->right_fork.taken = 0;
	tphilo->status = SLEEPING;
	(*turns)++;
	ft_usleep(tphilo->main_philo->eat_time);
}

int	loop_check(t_philo *tphilo, int *turns)
{
	tphilo->actual_time = get_time();
	return (tphilo->main_philo->playing
		&& (*turns) < tphilo->main_philo->turns);
}

void	initialize_fork_mutex(t_main *main, int index)
{
	main->forks_mutexes[index].f_mutex = malloc(sizeof(pthread_mutex_t));
	main->forks_mutexes[index].order = index;
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
	philo->main_philo->forks_mutexes[index].f_mutex
		= malloc(sizeof(pthread_mutex_t *));
	philo->start_eating = get_time();
	philo->left_fork = main->forks_mutexes[index];
	philo->right_fork = main->forks_mutexes[index % main->num_of_philos];
}

int	initialize_main(t_main *main, int args, char **argv)
{
	main->num_of_philos = ft_atoi(argv[1]);
	main->die_time = ft_atoi(argv[2]);
	main->eat_time = ft_atoi(argv[3]);
	main->sleep_time = ft_atoi(argv[4]);
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
		main->turns = ft_atoi(argv[5]);
	}
	main->table = malloc(main->num_of_philos * sizeof(t_philo));
	main->forks_mutexes = malloc(main->num_of_philos * sizeof(t_fork));
	main->print_turn = malloc(sizeof(pthread_mutex_t));
	main->playing = 1;
	main->initial_time = get_time();
	return (0);
}
