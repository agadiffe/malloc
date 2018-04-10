#include "malloc.h"
#include <sys/mman.h>

void		join_next_chunk(t_header *block)
{
	block->size += HEADER_SIZE + block->next->size;
	block->next = block->next->next;
	if (block->next)
		block->next->prev = block;
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
	new->next = tmp->next;
	if (new->next)
		new->next->prev = new;
	tmp->size = size;
	tmp->next = new;
	tmp->is_free = 0;
}

t_header	*create_chunk(size_t size)
{
	t_header	*tmp;

	if ((tmp = (t_header *)mmap(0, size, PROT, MAP, -1, 0)) == MAP_FAILED)
		return (NULL);
	else
	{
		tmp->mem = tmp + HEADER_SIZE;
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
