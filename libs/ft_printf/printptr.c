/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printptr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mathou <mathou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 18:12:30 by mavallee          #+#    #+#             */
/*   Updated: 2024/12/08 02:17:38 by mathou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

void	printptr(unsigned long long ptr, int *count)
{
	if (ptr >= 16)
	{
		printptr(ptr / 16, count);
		printptr(ptr % 16, count);
	}
	else
	{
		if (ptr >= 10)
			(*count) += ft_putc(ptr - 10 + 'a');
		else
			(*count) += ft_putc(ptr + '0');
	}
}

void	putptr(unsigned long long value, int *count)
{
	if (value == 0)
		(*count) += write(1, "(nil)", 5);
	else
	{
		ft_putstr_int("0x", count);
		printptr(value, count);
	}
}
