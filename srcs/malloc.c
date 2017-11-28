#include "malloc.h"
#include "libft.h"

void		print_zone(int zone)
{
	if (zone == 0)
		ft_putstr("TINY : ");
	else if (zone == 1)
		ft_putstr("SMALL : ");
	else
		ft_putstr("LARGE : ");
}

size_t		print_chunk(t_header **block, int zone)
{
	t_header	*tmp;
	size_t		zone_size;

	zone_size = 0;
	tmp = *block;
	ft_putstr("0x");
	print_zone(zone);
	while (tmp)
	{
		zone_size += tmp->size;
		ft_putstr("0x");
		ft_putnbr_base(tmp->mem, BASE16);
		ft_putstr(" - ");
		ft_putstr("0x");
		ft_putnbr_base(tmp->mem + tmp->size, BASE16);
		ft_putstr(" : ");
		ft_putnbr(tmp->size);
		ft_putendl(" octets");
		tmp = tmp->next;
	}
	return (zone_size);
}

void		show_alloc_mem(void)
{
	t_header	**data[4];
	t_header	*tmp;
	int			i;
	size_t		total_size;

	i = 0;
	total_size = 0;
	data[0] = &g_data.tiny;
	data[1] = &g_data.small;
	data[2] = &g_data.large;
	data[3] = NULL;
	while (data[i])
	{
		tmp = *data[i];
		if (tmp)
			total_size += print_chunk(&tmp, i);
		i++;
	}
	ft_putstr("Total : ");
	ft_putnbr(total_size);
	ft_putendl(" octets");
}

t_header	*find_chunk(void *ptr, int *zone)
{
	t_header	**data[4];
	t_header	*tmp;
	int			i;

	i = 0;
	data[0] = &g_data.tiny;
	data[1] = &g_data.small;
	data[2] = &g_data.large;
	data[3] = NULL;
	while (data[i])
	{
		if (zone)
			*zone = i;
		tmp = *data[i];
		while (tmp)
		{
			if (tmp->mem == ptr)
				return (tmp);
			tmp = tmp->next;
		}
		i++;
	}
	return (NULL);
}

void		join_next_chunk(t_header *block)
{
	block->size += HEADER_SIZE + block->next->size;
	block->next = block->next->next;
	if (block->next)
		block->next->prev = block;
}

void		handle_munmap(t_header *block, t_header **data)
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
	MUNMAP(block->mem - HEADER_SIZE, block->size + HEADER_SIZE);
}

void		handle_free(t_header *block, int zone)
{
	if (zone == 2)
	{
		MUNMAP(block->mem - HEADER_SIZE, block->size + HEADER_SIZE);
		g_data.large = NULL;
	}
	else if (zone == 1 && block->size > SMALL_ZONE)
		handle_munmap(block, &g_data.small);
	else if (block->size > TINY_ZONE)
		handle_munmap(block, &g_data.tiny);
}

void		free(void *ptr)
{
	t_header	*tmp;
	int			zone;

	if (!ptr)
		return ;
	pthread_mutex_lock(&g_mutex);
	zone = 0;
	if ((tmp = find_chunk(ptr, &zone)))
	{
		tmp->is_free = 1;
		if (tmp->prev && tmp->prev->is_free)
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

t_header	*find_free_chunk(t_header **lst, size_t size)
{
	t_header	*tmp;

	tmp = *lst;
	while (tmp && !(tmp->is_free && tmp->size >= size))
		tmp = tmp->next;
	return (tmp);
}

void		split_chunk(t_header **block, size_t size)
{
	t_header	*tmp;
	t_header	*new;

	tmp = *block;
	new = tmp->mem + size;
	new->mem = new + HEADER_SIZE;
	new->size = tmp->size - size - HEADER_SIZE;
	new->is_free = 1;
	new->prev = tmp;
	new->next = tmp->next ? tmp->next : NULL;
	tmp->size = size;
	tmp->next = new;
}

t_header	*create_chunk(size_t size)
{
	t_header	*tmp;

	if ((tmp = (t_header *)MMAP(size)) == MAP_FAILED)
		return (NULL);
	else
	{
		tmp->mem = tmp + HEADER_SIZE;
		tmp->is_free = 0;
		tmp->size = size - HEADER_SIZE;
		return (tmp);
	}
}

void		insert_chunk_ascending(t_header **data, t_header *ptr)
{
	t_header	*tmp;
	t_header	*save;

	tmp = *data;
	save = NULL;
	while (tmp && ptr > tmp)
	{
		save = tmp;
		tmp = tmp->next;
	}
	if (tmp)
	{
		ptr->next = tmp;
		tmp->prev->next = ptr;
		ptr->prev = tmp->prev;
		tmp->prev = ptr;
	}
	else
	{
		ptr->prev = save;
		save->next = ptr;
		ptr->next = NULL;
	}
}

t_header	*handle_malloc(size_t size, size_t zone, t_header **data)
{
	t_header	*ptr;

	if (!(ptr = find_free_chunk(data, size + HEADER_SIZE)))
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
	if (ptr->size - size >= HEADER_SIZE + 4)
		split_chunk(&ptr, size);
	return (ptr->mem);
}

void	*malloc(size_t size)
{
	void	*ptr;
	size_t	align_size;

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
	return (ptr == MAP_FAILED ? NULL : ptr);
}

void	*handle_realloc(void **ptr, size_t size)
{
	void		*newptr;
	t_header	*tmp;

	if (!(tmp = find_chunk(ptr, NULL)))
		return (NULL);
	if (tmp->size >= size)
	{
		if (tmp->size - size >= HEADER_SIZE + 4)
			split_chunk(&tmp, size);
	}
	else if (tmp->next && tmp->next->is_free
				&& tmp->size + HEADER_SIZE + tmp->next->size >= size)
	{
		join_next_chunk(tmp);
		if (tmp->size - size >= HEADER_SIZE + 4)
			split_chunk(&tmp, size);
	}
	else
	{
		if (!(newptr = malloc(size)))
			return (NULL);
		ft_memcpy(newptr, tmp->mem, tmp->size);
		return (newptr);
	}
	return (*ptr);
}

void	*realloc(void *ptr, size_t size)
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

void	*calloc(size_t nmemb, size_t size)
{
	void	*new;

	if (nmemb && size && (new = malloc(nmemb * size)))
	{
		ft_bzero(new, ALIGN4(nmemb * size));
		return (new);
	}
	return (NULL);
}

void	*reallocf(void *ptr, size_t size)
{
	void	*new;

	new = realloc(ptr, size);
	if (!new)
		free(ptr);
	return (new);
}
