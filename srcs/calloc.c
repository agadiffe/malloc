/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadiffe <agadiffe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/13 17:28:34 by agadiffe          #+#    #+#             */
/*   Updated: 2018/04/13 17:28:36 by agadiffe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void			*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)b;
	while (len)
	{
		*ptr++ = (unsigned char)c;
		len--;
	}
	return (b);
}

static void			ft_bzero(void *s, size_t n)
{
	ft_memset(s, '\0', n);
}

FOR_EXPORT_VOID		*calloc(size_t nmemb, size_t size)
{
	void	*new;

	pthread_mutex_init(&g_mutex, NULL);
	pthread_mutex_lock(&g_mutex);
	if (nmemb && size && (new = malloc(nmemb * size)))
	{
		ft_bzero(new, ALIGN4(nmemb * size));
		return (new);
	}
	pthread_mutex_unlock(&g_mutex);
	pthread_mutex_destroy(&g_mutex);
	return (NULL);
}
