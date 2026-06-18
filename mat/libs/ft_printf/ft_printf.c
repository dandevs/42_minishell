/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 18:01:42 by mavallee          #+#    #+#             */
/*   Updated: 2026/03/26 00:41:24 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

/*
	GOOD %c Prints a single character.

	GOOD %s Prints a string (as defined by the common C convention).

	%p The void * pointer argument has to be printed in hexadecimal format.

	%d Prints a decimal (base 10) number.

	%i Prints an integer in base 10.

	%u Prints an unsigned decimal (base 10) number.

	%x Prints a number in hexadecimal (base 16) lowercase format.

	%X Prints a number in hexadecimal (base 16) uppercase format.

	GOOD %% Prints a percent sign.
*/

int	ft_putc(int c)
{
	char	ch;

	ch = c;
	write(1, &ch, 1);
	return (1);
}

int	formatspec(char c, va_list args, int *count)
{
	if (c == '%')
		(*count) += ft_putc('%');
	else if (c == 'c')
		(*count) += ft_putc(va_arg(args, int));
	else if (c == 's')
		ft_putstr_int(va_arg(args, char *), count);
	else if (c == 'p')
		putptr(va_arg(args, unsigned long long), count);
	else if (c == 'd' || c == 'i')
		putdec(va_arg(args, int), count);
	else if (c == 'u')
		putuns(va_arg(args, unsigned int), count);
	else if (c == 'x' || c == 'X')
		puthex(va_arg(args, unsigned int), c, count);
	else
		return (0);
	return (1);
}

int	ft_printf(const char *fmt, ...)
{
	va_list	args;
	int		count;

	count = 0;
	va_start(args, fmt);
	if (!fmt)
		return (va_end(args), -1);
	while (*fmt != '\0')
	{
		if (*fmt != '%')
		{
			ft_putc(*fmt);
			fmt++;
			count++;
			continue ;
		}
		fmt++;
		if (!formatspec(*fmt, args, &count))
			return (count);
		fmt++;
	}
	va_end(args);
	return (count);
}
