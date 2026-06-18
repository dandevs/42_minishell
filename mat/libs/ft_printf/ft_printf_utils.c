/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 21:31:03 by mavallee          #+#    #+#             */
/*   Updated: 2026/06/04 16:37:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

void	ft_putstr_int(char *str, int *count)
{
	int	i;

	i = 0;
	if (!str)
	{
		write(1, "(null)", 6);
		(*count) += 6;
		return ;
	}
	while (str[i])
	{
		*count += ft_putc(str[i]);
		i++;
	}
}

/*static int	count_int_len(int n)
{
	int	len;

	len = 0;
	if (n < 0)
		len++;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*itoa;
	int		len;
	int		sign;

	sign = 1;
	itoa = malloc(sizeof(char) * (count_int_len(n) + 1));
	if (!itoa)
	{
		return (NULL);
	}
	if (n < 0)
	{
		itoa[0] = '-';
		sign = -1;
	}
	len = count_int_len(n) - 1;
	itoa[count_int_len(n)] = '\0';
	while ((len != -1 && sign == 1) || (len != 0 && sign == -1))
	{
		itoa[len] = ft_abs(n % 10) + '0';
		n /= 10;
		len--;
	}
	return (itoa);
}*/
