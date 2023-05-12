/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albgonza <albgonza@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:53:22 by albgonza          #+#    #+#             */
/*   Updated: 2023/05/12 18:45:26 by albgonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_arguments(int args, char **argv, t_main *main)
{
	ft_atoi(argv[1], main);
	ft_atoi(argv[2], main);
	ft_atoi(argv[3], main);
	ft_atoi(argv[4], main);
	if (args == 6)
		ft_atoi(argv[5], main);
	if (!main->valid_arguments)
		return (1);
	else
		return (0);
}
