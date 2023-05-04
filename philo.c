/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgonza <albgonza@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 12:31:42 by albgonza          #+#    #+#             */
/*   Updated: 2023/04/24 19:26:59 by albgonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
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

	tphilo = philo;
	tphilo->actual_time = 0;
	(void) turns;

	while (1)
	{
		if (!tphilo->left_fork.taken && !tphilo->right_fork.taken)
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
			philo_print("%lld %d is thinking\n", tphilo);
			tphilo->status = THINKING;
		}
		if (tphilo->status == EATING)
		{
 			tphilo->time_since_last_meal = 0;
			philo_print("%lld %d is eating\n", tphilo);
			pthread_mutex_unlock(tphilo->left_fork.f_mutex);
			pthread_mutex_unlock(tphilo->right_fork.f_mutex);
			tphilo->left_fork.taken = 0;
			tphilo->right_fork.taken = 0;
			tphilo->status = SLEEPING;
			philo_print("%lld %d is sleeping\n", tphilo);
		}
		else if (tphilo->status == SLEEPING)
		{
			
		}
		
		/* 
			Comprobar que para tomar los tenedores el
			filosofo debe estar en un estado que no sea SLEEPING, también
			comprobar porqué no se suman los ms al contador. Los impares 
			inician en estado SLEEPING y los pares en estado THINKING, esto produce 
			que sólo los pares coman, haciendo que los impares no coman nunca, 
			aunque cojan los tenedores que le tocan. Quizás la solución es
			contar el tiempo que están durmiendo los impares, y entonces
			pasan a estado THINKING, teniendo posible el coger
			los tenedores y comer.
		 */
	}
	return (NULL);
}

void	*time_management(t_main *main)
{
	t_main		*rmain;
	int			turns;
	int			i;
	/* long long	newtime;
	long long	time; */

	turns = 0;
	rmain = main;
	i = 0;
	// time = get_time();
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
	while (rmain->playing && turns < rmain->turns)
	{
		/* while (i < rmain->num_of_philos)
		{
			newtime = get_time();
			rmain->table[i].actual_time += newtime - time;
			if (rmain->table[i].status == EATING)
			{
				rmain->table[i]
					.time_since_last_meal = 0;
				philo_print("%lld %d is sleeping\n", &rmain->table[i]);
				rmain->table[i].status = SLEEPING;
			}
			else if (!rmain->table[i].left_fork.taken
				&& !rmain->table[i].right_fork.taken)
			{
				pthread_mutex_lock(rmain->table[i].left_fork.f_mutex);
				pthread_mutex_lock(rmain->table[i].right_fork.f_mutex);
				rmain->table[i].left_fork.taken = 1;
				rmain->table[i].right_fork.taken = 1;
				philo_print("%lld %d is eating\n", &rmain->table[i]);
				rmain->table[i].status = EATING;
				while (rmain->table[i].status == EATING)
				{
					newtime = get_time();
					rmain->table[i].actual_time += newtime - time;
					rmain->table[i].time_since_last_meal
						+= rmain->table[i].actual_time;
					if (rmain->table[i].time_since_last_meal > rmain->die_time)
					{
						rmain->playing = 0;
						philo_print("%lld %d died\n", &rmain->table[i]);
						pthread_mutex_unlock(rmain->table[i].left_fork.f_mutex);
						pthread_mutex_unlock(rmain
							->table[i].right_fork.f_mutex);
						break ;
					}
				}
			}
			else
			{
				philo_print("%lld %d is thinking\n", &rmain->table[i]);
				rmain->table[i].status = THINKING;
			}
			i++; 
		}*/
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
		p.main_philo->forks_mutexes[i].f_mutex = malloc(sizeof(pthread_mutex_t *));
		p.left_fork = main->forks_mutexes[i];
		if (i != 0)
			p.right_fork = main->forks_mutexes[i - 1];
		else
			p.right_fork = main->forks_mutexes[main->num_of_philos - 1];
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
