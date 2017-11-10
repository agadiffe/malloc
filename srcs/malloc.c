#include "malloc.h"

t_header	*find_chunk(void *ptr)
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

void		join_chunk(t_header **block)
{
	t_header	*tmp;

	tmp = *block;
	if (tmp->next && tmp->next->is_free)
	{
		tmp->size += HEADER_SIZE + tmp->next->size;
		tmp->next = tmp->next->next;
		if (tmp->next)
			tmp->next->prev = tmp;
	}
}

void		free(void *ptr)
{
	t_header	*tmp;

	if ((tmp = find_chunk(ptr)))
	{
		tmp->is_free = 1;
		if (tmp->prev && tmp->prev->is_free)
			join_chunk(&tmp);
		if (tmp->next)
			join_chunk(&tmp);
	}
	//todo:
	//check if size left > TINY or SMALL and call munmap;
	//if large, always munmap ?!
}

t_header	*find_free_chunk(t_header **lst, t_header **last, size_t size)
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

t_header	*handle_malloc(size_t size, size_t zone, t_header **data)
{
	t_header	*ptr;
	t_header	*last;

	if (!(ptr = find_free_chunk(data, &last, size + HEADER_SIZE)))
		ptr = create_chunk(zone, data, last);
	if (!ptr)
		return (NULL);
	if (last)
	{
		last->next = ptr;
		ptr->prev = last;
	}
	else
		*data = ptr;
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
		ptr = handle_malloc(align_size, TINY, &g_data.tiny);
	else if (align_size <= SMALL - HEADER_SIZE)
		ptr = handle_malloc(align_size, SMALL, &g_data.small);
	else 
		ptr = handle_malloc(align_size, align_size, &g_data.large);
	pthread_mutex_unlock(&g_mutex);
	return (ptr == MAP_FAILED ? NULL : ptr);
}
