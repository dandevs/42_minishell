/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printuns.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mathou <mathou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 19:39:47 by mavallee          #+#    #+#             */
/*   Updated: 2024/12/08 02:17:45 by mathou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

void	putuns(unsigned int nb, int *count)
{
	if (nb >= 10)
	{
		putuns(nb / 10, count);
		putuns(nb % 10, count);
	}
	else
		(*count) += ft_putc(nb % 10 + '0');
}
