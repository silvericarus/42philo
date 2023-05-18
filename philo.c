/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgonza <albgonza@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 12:31:42 by albgonza          #+#    #+#             */
/*   Updated: 2023/05/18 18:14:51 by albgonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_main_loop(void *philo)
{
	t_philo		*tphilo;
	int			turns;
	long long	die_alarm;

	tphilo = philo;
	tphilo->actual_time = 0;
	die_alarm = get_time() + tphilo->main_philo->die_time;
	turns = 0;
	while (loop_check(tphilo, &turns, &die_alarm))
	{
		if (should_take_forks(tphilo))
			take_forks(tphilo);
		else if (tphilo->left_fork->taken || tphilo->right_fork->taken)
			tphilo->status = THINKING;
		if (tphilo->status == EATING)
			finish_eating(tphilo, &die_alarm, &turns);
		if (tphilo->status == SLEEPING)
			sleep_and_think(tphilo, &die_alarm);
		if (tphilo->status == THINKING && tphilo->main_philo->playing)
			start_thinking(tphilo, &die_alarm);
	}
	return (NULL);
}

void	*time_management(t_main *main)
{
	t_main		*rmain;
	int			i;

	rmain = main;
	i = 0;
	while (i < rmain->num_of_philos)
	{
		if (pthread_create(&rmain->table[i].thread, NULL,
				philo_main_loop, &rmain->table[i]))
			printf("error creating thread\n");
		i++;
	}
	i = 0;
	while (i < rmain->num_of_philos)
	{
		if (pthread_join(rmain->table[i].thread, NULL))
			printf("error joining thread\n");
		i++;
	}
	return (NULL);
}

void	create_philos(t_main *main)
{
	int		i;
	t_philo	philo;

	i = 0;
	while (i < main->num_of_philos)
	{		
		initialize_fork_mutex(main, i);
		initialize_philo(&philo, main, i);
		main->table[i] = philo;
		i++;
	}
	main->died_printed = 0;
	main->printed_thinking = 0;
	main->died_print_m = malloc(sizeof(pthread_mutex_t));
	main->printed_thinking_m = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(main->died_print_m, NULL);
	pthread_mutex_init(main->print_turn, NULL);
	pthread_mutex_init(main->printed_thinking_m, NULL);
}

// void	ft_leaks(void)
// {
// 	system("leaks -q philo");
// }
// atexit(ft_leaks);

int	main(int args, char **argv)
{
	t_main	main;

	if (args == 5 || args == 6)
	{
		main.valid_arguments = 1;
		if (check_arguments(args, argv, &main) != 0)
			return (1);
		if (initialize_main(&main, args, argv) != 0)
			return (1);
		create_philos(&main);
		time_management(&main);
		ft_usleep(10);
		ft_free(&main);
		return (0);
	}
}
