/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadiffe <agadiffe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/13 17:29:05 by agadiffe          #+#    #+#             */
/*   Updated: 2018/04/13 17:29:12 by agadiffe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <sys/mman.h>

static t_header		*handle_malloc(size_t size, size_t zone, t_header **data)
{
	t_header	*ptr;

	if (!(ptr = find_free_chunk(data, size)))
	{
		if ((ptr = create_chunk(zone)))
		{
			if (!*data)
			{
				*data = ptr;
				ptr->prev = NULL;
				ptr->next = NULL;
			}
			else
				insert_chunk_ascending(data, ptr);
		}
		else
			return (NULL);
	}
	if ((intmax_t)(ptr->size - size) >= (intmax_t)(HEADER_SIZE + 4))
		split_chunk(ptr, size);
	else
		ptr->is_free = 0;
	return (ptr->mem);
}

FOR_EXPORT_VOID		*malloc(size_t size)
{
	void	*ptr;
	size_t	asize;
	pthread_mutex_t		*lock;

	lock = ft_memlock();
	pthread_mutex_lock(lock);
	asize = ALIGN4(size);
	if (!size)
		return (NULL);
	else if (asize <= TINY)
		ptr = handle_malloc(asize, TINY_ZONE, &g_data.tiny);
	else if (asize <= SMALL)
		ptr = handle_malloc(asize, SMALL_ZONE, &g_data.small);
	else
		ptr = handle_malloc(asize, asize + HEADER_SIZE, &g_data.large);
	pthread_mutex_unlock(lock);
	return (ptr == MAP_FAILED ? NULL : ptr);
}
