/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgonza <albgonza@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 19:10:06 by albgonza          #+#    #+#             */
/*   Updated: 2023/01/24 19:17:45 by albgonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(int str)
{
	if (str < '0' || str > '9')
		return (0);
	return (1);
}

int	ft_atoi(const char *str)
{
	int		i;
	int		neg;
	long	num;

	i = 0;
	num = 0;
	neg = 1;
	while (str[i] == ' ' || (str[i] >= 8 && str[i] <= 13))
		i++;
	if (str[i] == '-')
		neg *= -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (ft_isdigit(str[i]))
	{
		if (num * neg > 2147483647)
			return (-1);
		else if (num * neg < -2147483648)
			return (0);
		else
			num = num * 10 + (str[i] - '0');
		i++;
	}
	return (num * neg);
}
