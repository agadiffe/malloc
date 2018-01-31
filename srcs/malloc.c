#include "malloc.h"
#include <sys/mman.h>

static t_header		*handle_malloc(size_t size, size_t zone, t_header **data)
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

void				*malloc(size_t size)
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
