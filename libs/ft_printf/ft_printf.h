/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:08:00 by mavallee          #+#    #+#             */
/*   Updated: 2026/06/04 16:38:38 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>

// 				Prend en arg un seul B :

//										Fonction(s) de libft modifiee(s) :

// 				Prend en arg un seul B :

// Le bit est un caractere a changer

// 				Renvoie size_t de plsrs bit :

//char	*ft_itoa(int n);
void	ft_putstr_int(char *str, int *count);
int		ft_putc(int c);

// printf_utils :

void	ft_putnbr_fd(int n, int fd);
int		formatspec(char c, va_list args, int *count);
int		ft_printf(const char *fmt, ...);

void	putdec(int nb, int *count);
void	puthex(unsigned int nb, char c, int *count);
void	putuns(unsigned int nb, int *count);
void	putptr(unsigned long long value, int *count);

#endif
