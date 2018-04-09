#include "malloc.h"
#include <sys/mman.h>
#include <stddef.h>

static void		handle_munmap(t_header *block, t_header **data)
{
	if (block->prev)
	{
		block->prev->next = block->next;
		block->next->prev = block->prev;
	}
	else
	{
		*data = block->next;
		block->next->prev = NULL;
	}
	munmap(block->mem - HEADER_SIZE, block->size + HEADER_SIZE);
}

static void		handle_free(t_header *block, int zone)
{
	if (zone == 2)
	{
		munmap(block->mem - HEADER_SIZE, block->size + HEADER_SIZE);
		g_data.large = NULL;
	}
	// TODO: 2 * SMALL_ZONE is impossible
	else if (zone == 1 && block->size > 2 * SMALL_ZONE)
		handle_munmap(block, &g_data.small);
	else if (block->size > 2 * TINY_ZONE)
		handle_munmap(block, &g_data.tiny);
}

FOR_EXPORT_VOID			free(void *ptr)
{
	t_header	*tmp;
	int			zone;

	ft_putstr("FREE:\t");
	ft_putnbr_base((uintmax_t)ptr, BASE16);
	ft_putstr("\n");
	if (!ptr)
		return ;
	pthread_mutex_lock(&g_mutex);
	zone = 0;
	if ((tmp = find_chunk(ptr, &zone)))
	{
		tmp->is_free = 1;
		if (tmp->prev && tmp->prev->is_free) // TODO: Verifie qu'ils sont contigu
		{
			join_next_chunk(tmp->prev);
			tmp = tmp->prev;
		}
		if (tmp->next && tmp->next->is_free)
			join_next_chunk(tmp);
		if (zone)
			handle_free(tmp, zone);
	}
	pthread_mutex_unlock(&g_mutex);
}
