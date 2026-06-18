/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printdec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mathou <mathou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 18:51:20 by mavallee          #+#    #+#             */
/*   Updated: 2024/12/08 02:17:23 by mathou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

void	printdec(int nb, int sign, int *count)
{
	if (nb >= 10 || nb <= -10)
		printdec(nb / 10, sign, count);
	(*count) += ft_putc(sign * (nb % 10) + '0');
}

void	putdec(int nb, int *count)
{
	int	sign;

	sign = 1;
	if (nb < 0)
	{
		sign = -1;
		(*count) += ft_putc('-');
	}
	printdec(nb, sign, count);
}
