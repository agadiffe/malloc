/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadiffe <agadiffe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/13 17:29:21 by agadiffe          #+#    #+#             */
/*   Updated: 2018/04/13 17:29:25 by agadiffe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void			*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*s1;
	unsigned char	*s2;

	if (!n || (!dst && !src))
		return (dst);
	s1 = (unsigned char*)dst;
	s2 = (unsigned char*)src;
	while (n--)
		*s1++ = *s2++;
	return (dst);
}

static void			*fresh_malloc(t_header *tmp, size_t size)
{
	void				*newptr;
	pthread_mutex_t		*lock;

	lock = ft_memlock();
	pthread_mutex_unlock(lock);
	if (!(newptr = malloc(size)))
		return (NULL);
	pthread_mutex_lock(lock);
	ft_memcpy(newptr, tmp->mem, tmp->size);
	pthread_mutex_unlock(lock);
	return (newptr);
}

static void			*handle_realloc(void **ptr, size_t size)
{
	t_header			*tmp;
	pthread_mutex_t		*lock;

	lock = ft_memlock();
	pthread_mutex_lock(lock);
	if (!(tmp = find_chunk(*ptr, NULL)))
		return (NULL);
	if (tmp->size >= size)
	{
		if (tmp->size - size >= HEADER_SIZE + 4)
			split_chunk(tmp, size);
	}
	else if (tmp->next && tmp->next->is_free && tmp->size + HEADER_SIZE
			+ tmp->next->size >= size && tmp->mem + tmp->size == tmp->next)
	{
		join_next_chunk(tmp);
		if (tmp->size - size >= HEADER_SIZE + 4)
			split_chunk(tmp, size);
	}
	else
		return (fresh_malloc(tmp, size));
	pthread_mutex_unlock(lock);
	return (*ptr);
}

FOR_EXPORT_VOID		*realloc(void *ptr, size_t size)
{
	void	*ret;
	ret = NULL;
	if (!ptr)
		return (malloc(size));
	if (!size && ptr)
	{
		if (!(ret = malloc(1)))
			return (NULL);
		free(ptr);
	}
	else
	{
		if (!(ret = handle_realloc(&ptr, ALIGN4(size))))
			return (NULL);
		free(ptr);
	}
	return (ret);
}
