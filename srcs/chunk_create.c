#include "malloc.h"
#include <sys/mman.h>

void		join_next_chunk(t_header *block)
{
	block->size += HEADER_SIZE + block->next->size;
	block->next = block->next->next;
	if (block->next)
		block->next->prev = block;
}

void		split_chunk(t_header *block, size_t size)
{
	t_header	*new;

	new = (t_header *)(block->mem + size);
	ft_putendl("bug");
	new->mem = (void *)new + HEADER_SIZE;
	ft_putendl("bug");
	new->size = block->size - size - HEADER_SIZE;
	new->is_free = 1;
	new->prev = block;
	new->next = block->next;
	if (new->next)
		new->next->prev = new;
	block->size = size;
	block->next = new;
	block->is_free = 0;
}

t_header	*create_chunk(size_t size)
{
	t_header	*tmp;

	size += HEADER_SIZE;
	if ((tmp = (t_header *)mmap(0, size, PROT, MAP, -1, 0)) == MAP_FAILED)
		return (NULL);
	else
	{
		tmp->mem = (void *)tmp + HEADER_SIZE;
		tmp->size = size;
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
