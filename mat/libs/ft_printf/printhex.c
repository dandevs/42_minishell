/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printhex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mathou <mathou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 18:52:52 by mavallee          #+#    #+#             */
/*   Updated: 2024/12/08 02:17:30 by mathou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

void	printhex(unsigned int nb, char c, int *count)
{
	if (nb >= 16)
	{
		printhex(nb / 16, c, count);
		printhex(nb % 16, c, count);
	}
	else
	{
		if (nb <= 9)
			(*count) += ft_putc(nb + '0');
		else
		{
			if (c == 'x')
				(*count) += ft_putc(nb - 10 + 'a');
			else
				(*count) += ft_putc(nb - 10 + 'A');
		}
	}
}

void	puthex(unsigned int nb, char c, int *count)
{
	if (nb == 0)
		(*count) += ft_putc('0');
	else
		printhex(nb, c, count);
}
