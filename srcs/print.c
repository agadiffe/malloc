#include "malloc.h"

static void			print_zone(int zone)
{
	if (zone == 0)
		ft_putstr("TINY : ");
	else if (zone == 1)
		ft_putstr("SMALL : ");
	else
		ft_putstr("LARGE : ");
}

static size_t		print_chunk(t_header *block, int zone)
{
	size_t		zone_size;

	zone_size = 0;
	print_zone(zone);
	ft_putstr("0x");
	ft_putnbr_base((uintmax_t)block, BASE16);
	ft_putendl("");
	while (block)
	{
		if (block->is_free == 0)
		{
			zone_size += block->size;
			ft_putstr("0x");
			ft_putnbr_base((uintmax_t)block->mem, BASE16);
			ft_putstr(" - ");
			ft_putstr("0x");
			ft_putnbr_base((uintmax_t)block->mem + block->size, BASE16);
			ft_putstr(" : ");
			ft_putnbr(block->size);
			ft_putendl(" octets ");
		}
		block = block->next;
	}
	return (zone_size);
}

FOR_EXPORT_VOID		show_alloc_mem(void)
{
	t_header	**data[4];
	t_header	*tmp;
	int			i;
	size_t		total_size;

	pthread_mutex_init(&g_mutex, NULL);;
	pthread_mutex_lock(&g_mutex);
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
			total_size += print_chunk(tmp, i);
		i++;
	}
	ft_putstr("Total : ");
	ft_putnbr(total_size);
	ft_putendl(" octets");
	pthread_mutex_unlock(&g_mutex);
	pthread_mutex_destroy(&g_mutex);
}
