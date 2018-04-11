#include "malloc.h"
#include <sys/mman.h>

static t_header		*handle_malloc(size_t size, size_t zone, t_header **data)
{
	t_header	*ptr;

	if (!(ptr = find_free_chunk(data, size)))
	{
		ft_putstr("CREATING CHUNK");
		ft_putstr("\n");

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

	ft_putstr("FOUND CHUNK AT\t");
	ft_putstr("0x");
	ft_putnbr_base((uintmax_t)ptr, BASE16);
	ft_putstr("\n");
	ft_putstr("0x");
	ft_putnbr_base((uintmax_t)ptr->mem, BASE16);
	ft_putstr("\n");

	if (ptr->size - size >= HEADER_SIZE + 4)
	{
		ft_putstr("SPLITTING CHUNK\t");
		ft_putstr("0x");
		ft_putnbr_base((uintmax_t)ptr->mem, BASE16);
		ft_putstr("\t");
		ft_putnbr(size);
		ft_putstr("\n");

		split_chunk(&ptr, size);
	}
	else
		ptr->is_free = 0;
	return (ptr->mem);
}

FOR_EXPORT_VOID			*malloc(size_t size)
{
	void	*ptr;
	size_t	align_size;

	ft_putstr("MALLOC:\t");
	ft_putnbr(size);
	ft_putstr("\n");
	pthread_mutex_lock(&g_mutex);
	align_size = ALIGN4(size);
	if (!size)
		return (NULL);
	else if (align_size <= TINY)
		ptr = handle_malloc(align_size, TINY_ZONE, &g_data.tiny);
	else if (align_size <= SMALL)
		ptr = handle_malloc(align_size, SMALL_ZONE, &g_data.small);
	else 
		ptr = handle_malloc(align_size, align_size, &g_data.large);
	pthread_mutex_unlock(&g_mutex);
	if (ptr == MAP_FAILED)
		ft_putstr("FAILED TO MAP OMGOMGOMG\n");
	else {
		ft_putstr("MALLOC RESULT:\t");
		ft_putstr("0x");
		ft_putnbr_base((uintmax_t)ptr, BASE16);
		ft_putstr("\n");
	}
	show_alloc_mem();
	return (ptr == MAP_FAILED ? NULL : ptr);
}
