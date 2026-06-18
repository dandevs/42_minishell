/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavallee <mavallee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 17:49:11 by mavallee          #+#    #+#             */
/*   Updated: 2024/06/01 16:02:05 by mavallee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_int_len(int n)
{
	int	len;

	len = 0;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static int	power_to(int n, int power)
{
	int	i;
	int	n_init;

	n_init = n;
	i = 2;
	if (power == 0)
		return (1);
	if (power == 1)
		return (n);
	while (i <= power)
	{
		n *= n_init;
		i++;
	}
	return (n);
}

void	ft_putnbr_fd(int n, int fd)
{
	char	c;
	int		len;
	char	minus;
	int		sign;

	if (fd < 0)
		return ;
	sign = 1;
	if (n < 0)
	{
		minus = '-';
		sign = -1;
		write(fd, &minus, 1);
	}
	len = count_int_len(n);
	while (len != 0)
	{
		c = sign * (n / power_to(10, len - 1)) + '0';
		n %= power_to(10, len - 1);
		write(fd, &c, 1);
		len--;
	}
}
/*
int	main(void)
{
	ft_putnbr_fd(0, 1);
	//printf("10 puissance 4 %d", power_to(10, 4));
}
*/