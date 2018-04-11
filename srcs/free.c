#include "malloc.h"
#include <sys/mman.h>
#include <stddef.h>

static void			handle_munmap(t_header *block, t_header **data)
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

static void			handle_free(t_header *block, int zone)
{
	return;
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
//18470
//static void		join_next_chunk(t_header *block)
//{
//	block->size += HEADER_SIZE + block->next->size;
//	block->next = block->next->next;
//	if (block->next)
//		block->next->prev = block;
//}
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
		// TODO: Verifie qu'ils sont contigu
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
