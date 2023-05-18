/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgonza <albgonza@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 19:10:06 by albgonza          #+#    #+#             */
/*   Updated: 2023/05/18 17:05:56 by albgonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(int str)
{
	if (str < '0' || str > '9')
		return (0);
	return (1);
}

int	ft_atoi(const char *str, t_main *main)
{
	int			i;
	int			neg;
	long long	num;

	i = 0;
	num = 0;
	neg = 1;
	while (str[i] == ' ' || (str[i] >= 8 && str[i] <= 13))
		i++;
	if (str[i] == '-')
		neg *= -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] != '\0')
	{
		if (!ft_isdigit(str[i]))
			main->valid_arguments = 0;
		if (num * neg > INT_MAX || num * neg < INT_MIN)
			main->valid_arguments = 0;
		else
			num = num * 10 + (str[i] - '0');
		i++;
	}
	return (num * neg);
}

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
		usleep(10);
}

void	philo_print(char *str, t_philo *philo)
{
	t_philo	*tphilo;

	tphilo = philo;
	philo->actual_time = get_time();
	pthread_mutex_lock(philo->main_philo->print_turn);
	pthread_mutex_lock(philo->main_philo->died_print_m);
	if (philo->main_philo->playing || (philo->status == DYING
			&& philo->main_philo->died_printed != 1))
	{
		pthread_mutex_unlock(philo->main_philo->died_print_m);
		printf(str, tphilo->actual_time
			- philo->main_philo->initial_time, philo->id);
		if (philo->status == DYING)
		{
			pthread_mutex_lock(philo->main_philo->died_print_m);
			philo->main_philo->died_printed++;
			pthread_mutex_unlock(philo->main_philo->died_print_m);
		}
	}
	pthread_mutex_unlock(philo->main_philo->died_print_m);
	pthread_mutex_unlock(philo->main_philo->print_turn);
}
