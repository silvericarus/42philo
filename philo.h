/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgonza <albgonza@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 12:32:07 by albgonza          #+#    #+#             */
/*   Updated: 2023/04/24 19:15:37 by albgonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <pthread.h>
# include <sys/time.h>

typedef enum e_states
{
	EATING,
	SLEEPING,
	THINKING,
	DYING,
}t_states;

typedef struct s_fork
{
	pthread_mutex_t	*f_mutex;
	int				order;
	int				taken;
}	t_fork;

typedef struct s_philo
{
	int				id;
	int				left_handed;
	t_fork			left_fork;
	t_fork			right_fork;
	t_states		status;
	long long		start_eating;
	long long		actual_time;
	long long		time_since_last_meal;
	pthread_t		thread;
	struct s_main	*main_philo;
}	t_philo;

typedef struct s_main
{
	t_philo				*table;
	int					num_of_philos;
	pthread_mutex_t		*print_turn;
	t_fork				*forks_mutexes;
	int					die_time;
	int					eat_time;
	int					sleep_time;
	int					turns;
	int					loop_mode;
	int					playing;
	long long			initial_time;
}	t_main;

int		ft_atoi(const char *str);
int		ft_isdigit(int str);
void	print_table(t_main *main);

#endif