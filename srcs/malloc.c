#include "malloc.h"

t_header	*find_chunk(t_header **lst, t_header **last, size_t size)
{
	t_header	*tmp;

	tmp = *lst;
	last = NULL;
	while (tmp && !(tmp->is_free && tmp->size >= size))
	{
		*last = tmp;
		tmp = tmp->next;
	}
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
	new->next = NULL;
	tmp->size = size;
	tmp->next = new;
}

void		fill_chunk(t_header **block, size_t size)
{
	t_header	*tmp;

	tmp = *block;
	tmp->mem = tmp + HEADER_SIZE;
	tmp->is_free = 0;
	tmp->size = size - HEADER_SIZE;
	tmp->prev = NULL;
	tmp->next = NULL;
}

t_header	*handle_mmap(size_t size, t_header **ptr)
{
	if ((*ptr = (t_header *)MMAP(size)) == MAP_FAILED)
		return (NULL);
	else
	{
		fill_chunk(ptr, size);
		return (*ptr);
	}
}

t_header	*create_chunk(size_t size, t_header **data, t_header *last)
{
	t_header	*tmp;

	tmp = *data;
	if (tmp)
		return (handle_mmap(size, &last->next));
	else
		return (handle_mmap(size, data));
}

t_header	*handle_small(size_t size)
{
	(void)size;
	return (NULL);
}

t_header	*handle_large(size_t size)
{
	(void)size;
	return (NULL);
}

t_header	*handle_tiny(size_t size)
{
	t_header	*ptr;
	t_header	*last;

	if (!(ptr = find_chunk(&g_data.tiny, &last, size + HEADER_SIZE)))
		ptr = create_chunk(TINY, &g_data.tiny, last);
	if (!ptr)
		return (NULL);
	if (last)
	{
		last->next = ptr;
		ptr->prev = last;
	}
	else
		g_data.tiny = ptr;
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
	if (size <= 0)
		return (NULL);
	else if (align_size <= TINY - HEADER_SIZE)
		ptr = handle_tiny(align_size);
	else if (align_size <= SMALL - HEADER_SIZE)
		ptr = handle_small(align_size);
	else 
		ptr = handle_large(align_size);
	pthread_mutex_unlock(&g_mutex);
	return (ptr == MAP_FAILED ? NULL : ptr);
}
