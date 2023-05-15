/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgonza <albgonza@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 19:34:38 by albgonza          #+#    #+#             */
/*   Updated: 2023/05/15 20:34:43 by albgonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_free(t_main *main)
{
	int	i;

	i = 0;
	free(main->print_turn);
	free(main->died_print_m);
	pthread_mutex_destroy(main->print_turn);
	pthread_mutex_destroy(main->died_print_m);
	while (i < main->num_of_philos)
	{
		pthread_mutex_destroy(main->forks_mutexes[i].f_mutex);
		free(main->forks_mutexes[i].f_mutex);
		i++;
	}
	free(main->forks_mutexes);
}

void	take_forks(t_philo *tphilo)
{
	philo_print("%lld %d has taken a fork\n", tphilo);
	pthread_mutex_lock(tphilo->left_fork->f_mutex);
	tphilo->left_fork->taken = 1;
	philo_print("%lld %d has taken a fork\n", tphilo);
	pthread_mutex_lock(tphilo->right_fork->f_mutex);
	tphilo->right_fork->taken = 1;
	tphilo->status = EATING;
}

void	sleep_and_think(t_philo *tphilo, long long *alarm, long long *die_alarm)
{
	if (tphilo->main_philo->playing)
		philo_print("%lld %d is sleeping\n", tphilo);
	*alarm = get_time() + tphilo->main_philo->sleep_time;
	while (tphilo->actual_time <= *alarm)
	{
		ft_usleep(100);
		tphilo->actual_time = get_time();
		if (!tphilo->main_philo->playing)
			break ;
		if (tphilo->actual_time > *die_alarm)
			handle_death(tphilo);
	}
	pthread_mutex_lock(tphilo->main_philo->printed_thinking_m);
	tphilo->main_philo->printed_thinking = 0;
	pthread_mutex_unlock(tphilo->main_philo->printed_thinking_m);
	tphilo->status = THINKING;
}

int	should_take_forks(t_philo *tphilo)
{
	return (!tphilo->left_fork->taken && !tphilo->right_fork->taken
		&& tphilo->status == THINKING
		&& tphilo->main_philo->num_of_philos > 1);
}

void	handle_death(t_philo *tphilo)
{
	tphilo->main_philo->playing = 0;
	tphilo->status = DYING;
	printf("%s", RED);
	philo_print("%lld %d died\n", tphilo);
	printf("%s", COLORNONE);
}
