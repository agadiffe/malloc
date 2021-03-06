/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadiffe <agadiffe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/13 17:28:47 by agadiffe          #+#    #+#             */
/*   Updated: 2018/04/13 17:28:55 by agadiffe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <sys/mman.h>

static void			handle_munmap(t_header *block, t_header **data, int zone)
{
	if ((zone == 1 && block->size == SMALL_ZONE - HEADER_SIZE
				&& (block->prev || block->next))
			|| (zone == 0 && block->size == TINY_ZONE - HEADER_SIZE
				&& (block->prev || block->next)))
	{
		if (block->prev)
		{
			block->prev->next = block->next;
			if (block->next)
				block->next->prev = block->prev;
		}
		else
		{
			*data = block->next;
			if (block->next)
				block->next->prev = NULL;
		}
		munmap(block, block->size + HEADER_SIZE);
	}
}

static void			handle_free(t_header *block, int zone)
{
	if (zone == 2)
	{
		munmap(block, block->size + HEADER_SIZE);
		g_data.large = NULL;
	}
	else if (zone == 1)
		handle_munmap(block, &g_data.small, 1);
	else
		handle_munmap(block, &g_data.tiny, 0);
}

FOR_EXPORT_VOID		free(void *ptr)
{
	t_header			*tmp;
	int					zone;
	pthread_mutex_t		*lock;

	if (!ptr)
		return ;
	lock = ft_memlock();
	pthread_mutex_lock(lock);
	zone = -1;
	if ((tmp = find_chunk(ptr, &zone)))
	{
		tmp->is_free = 1;
		if (tmp->next && tmp->next->is_free
				&& tmp->mem + tmp->size == tmp->next)
			join_next_chunk(tmp);
		if (tmp->prev && tmp->prev->is_free
				&& tmp->prev->mem + tmp->prev->size == tmp)
		{
			join_next_chunk(tmp->prev);
			tmp = tmp->prev;
		}
		handle_free(tmp, zone);
	}
	pthread_mutex_unlock(lock);
}
