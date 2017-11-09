#include "malloc.h"
#include <sys/mman.h>

t_header	*find_chunk(t_header **lst, t_header *last, size_t size)
{
	t_header	*tmp;

	tmp = *lst;
	while (tmp && !(tmp->free && tmp->size >= size))
	{
		*last = tmp;
		tmp = tmp->next;
	}
	return (tmp);
}

void		split_chunk(t_header **block, size_t size)
{
}

t_header	*create_tiny_chunk(size_t size)
{
}

void		fill_chunk(t_header **block)
{
}

t_header	*handle_tiny(size_t size)
{
	t_header	*ptr;

	ptr = find_chunk(&g_data.tiny, size + HEADER_SIZE);
	if (ptr && ptr->size - size >= HEADER_SIZE + 4)
		split_chunk(&ptr, size);
	else
		ptr = create_tiny_chunk(size);
	fill_chunk(&ptr);
}

void	*malloc(site_t size)
{
	void	*ptr;
	size_t	align_size;

	pthread_mutex_lock(&g_mutex);
	align_size = ALIGN4(size);
	if (size <= 0)
		return (NULL);
	else if (align_size <= TINY)
		ptr = handle_tiny(align_size);
	else if (align_size <= SMALL)
		ptr = handle_small(align_size);
	else 
		ptr = handle_large(align_size);
	pthread_mutex_unlock(&g_mutex);
	return (ptr == MAP_FAILED ? NULL : ptr);
}
