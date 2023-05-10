/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgonza <albgonza@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 12:31:42 by albgonza          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/05/10 21:59:39 by albgonza         ###   ########.fr       */
=======
/*   Updated: 2023/05/10 18:39:52 by albgonza         ###   ########.fr       */
>>>>>>> e66d72852ed77f38f1c85620ac176e2b80334298
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_main_loop(void *philo)
{
	t_philo		*tphilo;
	int			turns;
	long long	alarm;
	long long	die_alarm;

	tphilo = philo;
	tphilo->actual_time = 0;
	die_alarm = get_time() + tphilo->main_philo->die_time;
	turns = 0;
	while (tphilo->main_philo->playing && turns < tphilo->main_philo->turns)
	{
		if (!tphilo->left_fork.taken && !tphilo->right_fork.taken
			&& tphilo->status == THINKING
			&& tphilo->main_philo->num_of_philos > 1)
		{
			tphilo->left_fork.taken = 1;
			philo_print("%lld %d has taken a fork\n", tphilo);
			pthread_mutex_lock(tphilo->left_fork.f_mutex);
			tphilo->right_fork.taken = 1;
			philo_print("%lld %d has taken a fork\n", tphilo);
			pthread_mutex_lock(tphilo->right_fork.f_mutex);
			tphilo->status = EATING;
		}
		else if (tphilo->left_fork.taken || tphilo->right_fork.taken)
		{
			tphilo->status = THINKING;
		}
		if (tphilo->status == EATING)
		{
			die_alarm = get_time() + tphilo->main_philo->die_time;
			philo_print("%lld %d is eating\n", tphilo);
			pthread_mutex_unlock(tphilo->left_fork.f_mutex);
			pthread_mutex_unlock(tphilo->right_fork.f_mutex);
			tphilo->left_fork.taken = 0;
			tphilo->right_fork.taken = 0;
			ft_usleep(tphilo->main_philo->eat_time);
			tphilo->status = SLEEPING;
			turns++;
		}
		if (tphilo->status == SLEEPING)
		{
			if (tphilo->main_philo->playing)
				philo_print("%lld %d is sleeping\n", tphilo);
			alarm = get_time() + tphilo->main_philo->sleep_time;
			while (tphilo->actual_time <= alarm)
			{
				ft_usleep(10);
				tphilo->actual_time = get_time();
				if (!tphilo->main_philo->playing)
					break ;
			}
			tphilo->status = THINKING;
		}
		if (tphilo->status == THINKING && tphilo->main_philo->playing)
		{
			philo_print("%lld %d is thinking\n", tphilo);
		}
		if (tphilo->actual_time > die_alarm)
		{
			tphilo->main_philo->playing = 0;
			tphilo->status = DYING;
			philo_print("%lld %d died\n", tphilo);
		}
		tphilo->actual_time = get_time();
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
	t_philo	p;

	i = 0;
	while (i < main->num_of_philos)
	{
		if (i % 2 == 0)
		{
			p.status = SLEEPING;
			p.left_handed = 1;
		}
		else
		{
			p.status = THINKING;
			p.left_handed = 0;
		}
		p.id = i + 1;
		p.main_philo = (struct s_main *)main;
		p.start_eating = get_time();
		p.main_philo->forks_mutexes[i].f_mutex
			= malloc(sizeof(pthread_mutex_t *));
		p.left_fork = main->forks_mutexes[i];
		p.right_fork = main->forks_mutexes[i % main->num_of_philos];
		p.time_since_last_meal = 0;
		main->table[i] = p;
		main->forks_mutexes[i].f_mutex = malloc(sizeof(pthread_mutex_t));
		main->forks_mutexes[i].order = i;
		main->forks_mutexes[i].taken = 0;
		pthread_mutex_init(main->forks_mutexes[i].f_mutex, NULL);
		i++;
	}
	main->died_printed = 0;
	main->died_print_m = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(main->died_print_m, NULL);
	pthread_mutex_init(main->print_turn, NULL);
}

void	ft_leaks(void)
{
	system("leaks -q philo");
}

int	main(int args, char **argv)
{
	t_main	main;

	if (args == 5 || args == 6)
	{
		main.num_of_philos = ft_atoi(argv[1]);
		main.die_time = ft_atoi(argv[2]);
		main.eat_time = ft_atoi(argv[3]);
		main.sleep_time = ft_atoi(argv[4]);
		if (main.num_of_philos < 1 || main.num_of_philos > 200)
		{
			printf("There must be at least one philosopher");
			printf(" and no more than 200!!! >:(\n");
			return (1);
		}
		if (args == 5)
		{
			main.loop_mode = 0;
			main.turns = 42;
		}
		else
		{
			main.loop_mode = 1;
			main.turns = ft_atoi(argv[5]);
		}
		main.table = malloc(main.num_of_philos * sizeof(t_philo));
		main.forks_mutexes = malloc(main.num_of_philos
				* sizeof(t_fork));
		main.print_turn = malloc(sizeof(pthread_mutex_t));
		main.playing = 1;
		main.initial_time = get_time();
		create_philos(&main);
		time_management(&main);
		ft_usleep(40);
		ft_free(&main);
		atexit(ft_leaks);
	}
}
