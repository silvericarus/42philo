/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgonza <albgonza@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 12:31:42 by albgonza          #+#    #+#             */
/*   Updated: 2023/05/04 21:04:54 by albgonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	ft_usleep(long long ms)
{
	long long	time;

	time = get_time();
	while ((get_time() - time) < ms)
		usleep(5);
}

void	philo_test(char *str, t_philo *philo)
{
	t_philo	*tphilo;
	char	tmp[1];

	tphilo = philo;
	philo->actual_time = get_time();
	pthread_mutex_lock(tphilo->main_philo->print_turn);
	printf(str, tphilo->actual_time
		- philo->main_philo->initial_time, tphilo->id, tphilo->status);
	scanf("%s\n", tmp);
	pthread_mutex_unlock(tphilo->main_philo->print_turn);
}

void	philo_print(char *str, t_philo *philo)
{
	t_philo	*tphilo;

	tphilo = philo;
	philo->actual_time = get_time();
	pthread_mutex_lock(tphilo->main_philo->print_turn);
	printf(str, tphilo->actual_time
		- philo->main_philo->initial_time, tphilo->id);
	pthread_mutex_unlock(tphilo->main_philo->print_turn);
}

void	*philo_main_loop(void *philo)
{
	t_philo		*tphilo;
	int			turns;
	long long	alarm;
	long long	die_alarm;

	tphilo = philo;
	tphilo->actual_time = 0;
	die_alarm = get_time() + tphilo->main_philo->die_time;
	(void) turns;
	while (tphilo->main_philo->playing)
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
			philo_print("%lld %d died\n", tphilo);
		}
		tphilo->actual_time = get_time();
	}
	return (NULL);
}

void	*time_management(t_main *main)
{
	t_main		*rmain;
	int			turns;
	int			i;

	turns = 0;
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

void	print_table(t_main *main)
{
	int	i;

	i = 0;
	printf("Time to die: %d, time to eat: %d, time to sleep: %d\n",
		main->die_time, main->eat_time, main->sleep_time);
	if (main->loop_mode)
		printf("Times each philosopher must eat: %d\n", main->turns);
	while (i < main->num_of_philos)
	{
		printf("Philosopher %d: left handed = %d & is in a %u status.\n",
			main->table[i].id,
			main->table[i].left_handed, main->table[i].status);
		i++;
	}
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
	pthread_mutex_init(main->print_turn, NULL);
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
		if (main.num_of_philos < 1)
		{
			printf("There must be at least one philosopher\n");
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
	}
}
