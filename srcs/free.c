#include "malloc.h"
#include <sys/mman.h>
#include <stddef.h>

static void			handle_munmap(t_header *block, t_header **data, int zone)
{
	ft_putendl("munmap");
	if (block->prev)
	{
		ft_putendl("block prev");
		block->prev->next = block->next;
		if (block->next)
			block->next->prev = block->prev;
		ft_putendl("end?");
	}
	else
	{
		ft_putendl("only 1 block");
		*data = block->next;
		if (block->next)
			block->next->prev = NULL;
		ft_putendl("end?");
	}
	//TODO: check if free space in block->prev or block->next
	if ((zone == 1 && block->size == SMALL_ZONE - HEADER_SIZE
				&& (block->prev || block->next))
			|| (zone == 0 && block->size == TINY_ZONE - HEADER_SIZE
				&& (block->prev || block->next)))
		munmap(block, block->size + HEADER_SIZE);
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
	t_header	*tmp;
	int			zone;

	ft_putstr("FREE:\t");
	ft_putnbr(HEADER_SIZE);
	ft_putstr(" 0x");
	ft_putnbr_base((uintmax_t)ptr, BASE16);
	ft_putstr("\n");
	if (!ptr)
		return ;
	pthread_mutex_lock(&g_mutex);
	zone = 0;
	if ((tmp = find_chunk(ptr, &zone)))
	{
		tmp->is_free = 1;
		ft_putstr("tmp->next:  0x");
		ft_putnbr_base((uintmax_t)tmp->next, BASE16);
		ft_putstr("\n");
		ft_putstr("tmp->next + HS:  0x");
		ft_putnbr_base((uintmax_t)tmp->next + HEADER_SIZE, BASE16);
		ft_putstr("\n");
		if (tmp->next && tmp->next->is_free
				&& tmp->mem + tmp->size == tmp->next)
		{
			ft_putendl("JOIN NEXT");
			join_next_chunk(tmp);
		}
		if (tmp->prev)
		{
			ft_putstr("tmp:  0x");
			ft_putnbr_base((uintmax_t)tmp, BASE16);
			ft_putstr("\n");
		}
		if (tmp->prev && tmp->prev->is_free
				&& tmp->prev->mem + tmp->prev->size == tmp)
		{
			ft_putendl("JOIN PREV");
			join_next_chunk(tmp->prev);
			tmp = tmp->prev;
		}
		if (zone)
			handle_free(tmp, zone);
		ft_putendl("OMG");
	}
	ft_putendl("OO");
	pthread_mutex_unlock(&g_mutex);
}
