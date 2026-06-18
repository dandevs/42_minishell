/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavallee <mavallee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 10:38:35 by mavallee          #+#    #+#             */
/*   Updated: 2024/06/05 14:45:15 by mavallee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_mult(size_t nmemb, size_t size)
{
	size_t	temp;

	temp = size;
	temp *= nmemb;
	if (size == 0 || nmemb == 0)
		return (1);
	if (temp / nmemb == size || temp / size == nmemb)
		return (1);
	return (0);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*dest;

	if (!ft_mult(nmemb, size))
		return (NULL);
	dest = malloc(size * nmemb);
	if (!dest)
	{
		return (NULL);
	}
	ft_memset(dest, 0, size * nmemb);
	return (dest);
}
