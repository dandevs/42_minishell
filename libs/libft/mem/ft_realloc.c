/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mathvall <mathvall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 18:12:31 by mathvall          #+#    #+#             */
/*   Updated: 2026/08/24 18:37:00 by mathvall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *mem, size_t nmemb, size_t size)
{
	void	*new;

	new = ft_calloc(nmemb, size);
	if (!new)
		return (NULL);
	ft_memcpy(new, mem, nmemb);
	free(mem);
	return (new);
}
