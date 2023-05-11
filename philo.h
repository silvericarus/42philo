/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgonza <albgonza@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 12:32:07 by albgonza          #+#    #+#             */
/*   Updated: 2023/05/11 22:28:13 by albgonza         ###   ########.fr       */
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
# define RED "\033[91m"
# define COLORNONE "\033[0m"

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
	pthread_mutex_t		*died_print_m;
	int					died_printed;
}	t_main;

//utils.c
int			ft_atoi(const char *str);
int			ft_isdigit(int str);
long long	get_time(void);
void		ft_usleep(long long ms);
void		philo_print(char *str, t_philo *philo);
//loop_split.c
void		ft_free(t_main *main);
void		take_forks(t_philo *tphilo);
void		sleep_and_think(t_philo *tphilo, long long *alarm,
				long long *die_alarm);
int			should_take_forks(t_philo *tphilo);
void		handle_death(t_philo *tphilo);
//loop_split_2.c
void		finish_eating(t_philo *tphilo, long long *die_alarm, int *turns);
int			loop_check(t_philo *tphilo, int *turns);
void		initialize_fork_mutex(t_main *main, int index);
void		initialize_philo(t_philo *philo, t_main *main, int index);
int			initialize_main(t_main *main, int args, char **argv);
#endif